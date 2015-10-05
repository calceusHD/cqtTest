#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <vector>
#include "gl/Program.h"
#include "font/Print.h"
#include <glm/glm.hpp>
#include "portaudio.h"
#include <cmath>
#include <cstdio>
#include "Kern.h"
#include "CQT.h"

#define mouseDown click && !clickPrev
#define mouseUp !click && clickPrev
#define mousePressed click
#define M2PI 6.28318530718


class Main {
    
    Print* print;
    Font* font;
    TextureManager * tm;
    Vec2<unsigned int> screen;
    bool isClicked;
    double *mAudioData;
    unsigned int mAudioLength;
    unsigned int mDataOffset;
    std::vector<glm::vec2> points;
    float mOffset;
    float mScale;
    public:
        
        static float wave(float freq, float sampleRate, float time) {
            return std::sin(M2PI * freq / sampleRate * time);
        }
    
        Main() 
        {
            mScale = 0.1f;
            mOffset = 1.0f;
            mDataOffset = 0;
            isClicked = false;
            screen = Vec2<unsigned int>(800, 600);
            std::setlocale(LC_ALL, "en_US.UTF-8");
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            GLFWwindow* window = glfwCreateWindow(screen.x, screen.y, "test", nullptr, nullptr);
            if (window == nullptr)
            {
                printf("cant create window");
                return;
            }
            
            glfwSetWindowSizeCallback(window, windowSizeCallback);
            glfwSetKeyCallback(window, keyCallback);
            glfwSetMouseButtonCallback(window, clickCallback);
            glfwSetCursorPosCallback(window, mouseCallback);
            glfwMakeContextCurrent(window);
            glewExperimental = true;
            glewInit();
            
            int tmp;

            glGetIntegerv(GL_MAX_ELEMENTS_VERTICES , &tmp);

            std::cout << "GL_MAX_ELEMENTS_VERTICES: " << tmp << std::endl;
            
            int err = Pa_Initialize();
            if (err != paNoError)
                printf("error");
            
            int num = Pa_GetDeviceCount();
            const PaDeviceInfo* devInfo;
            const PaHostApiInfo* apiInfo;
            for (int i = 0; i < num; ++i) {
                devInfo = Pa_GetDeviceInfo(i);
                apiInfo = Pa_GetHostApiInfo(devInfo->hostApi);
                printf("%i, %s on %s\n", i, devInfo->name, apiInfo->name);
            }
            
            
            float sampleRate = 44100.0f;
            
            
            double t = glfwGetTime();
            Kern k(sampleRate, 12, 4 * 16.352f, sampleRate / 2);
            BlockMatrix<std::complex<double>> b(k.K, k.mN0, k.mB, 0.01);
            mAudioData = new double[b.getWidth()];
            mAudioLength = b.getWidth();
            for (unsigned int i = 0; i < mAudioLength; ++i) {
                mAudioData[i] = wave(55, sampleRate, i) + wave(110, sampleRate, i) + wave(220, sampleRate, i)
                        + wave(440, sampleRate, i) + wave(880, sampleRate, i) + wave(1760, sampleRate, i)
                        + wave(3520, sampleRate, i) + wave(7040, sampleRate, i);
            }
            
            printf("kernel time:%f\n", glfwGetTime() - t);
            float drawArray[k.mB * 2];
            std::complex<double> out[k.mB];
            CQT::transform(mAudioData, out, b, mAudioLength);
            t = glfwGetTime();
            
            printf("transform time:%f\n", glfwGetTime() - t);
            
            //glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(debugCallback, nullptr);
            //glEnable(GL_DEBUG_OUTPUT);
                        
            printf("%s\n", glGetString(GL_VERSION));
            Shader fs("res/shader/fragment.c", true, GL_FRAGMENT_SHADER);
            Shader vs("res/shader/vertex.c", true, GL_VERTEX_SHADER);
            Program* p = new Program();
            p->attach(fs);
            p->attach(vs);
            p->build();
            p->use();
            
            Program p2;
            Shader fs2("res/shader/fragment2.c", true, GL_FRAGMENT_SHADER);
            Shader vs2("res/shader/vertex2.c", true, GL_VERTEX_SHADER);
            p2.attach(fs2);
            p2.attach(vs2);
            p2.build();
            p2.use();
            
            int uniformData = p2.getUniformLocation("data");
            
            unsigned int waterfallSize = 512;
            
            tm = new TextureManager();
            
            unsigned int waterfallTexture;
            unsigned int waterfallId = tm->getFreeTexture();
            
            glGenTextures(1, &waterfallTexture);
            glActiveTexture(GL_TEXTURE0 + waterfallId);
            
            glBindTexture( GL_TEXTURE0, waterfallTexture );

            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            unsigned char* textureTmp = new unsigned char[waterfallSize * b.getWidth()];
            
            glTexImage2D( GL_TEXTURE_2D_ARRAY, 0, GL_R8, b.getWidth(), waterfallSize, 0, GL_RED, GL_UNSIGNED_BYTE, textureTmp);
            
            delete textureTmp;
            
            float max = 0;
            for (unsigned int i = 0; i < k.mB; ++i) {
                    drawArray[2 * i + 0] = (float)i / k.mB * 2.0f - 1.0f;
                    float tmp = std::abs(out[i]);
                    drawArray[2 * i + 1] = tmp;
                    max = std::max(tmp, max);

                }
            
            font = new Font(512, "res/font/DroidSans.woff", 32, tm);
            print = new Print(font);
            //print.set(&font, "res/shader/fontVertex.c", "res/shader/fontFragment.c");
            print->setScreenSize(screen);
            glm::vec2* vert = new glm::vec2[1024];
            
            glm::vec2* debug = new glm::vec2[b.getWidth()];
            for (unsigned int i = 0; i < b.getWidth(); ++i) {
                debug[i].x = (float)i / b.getWidth() * 2.0f - 1.0f;
            }
            uint32_t vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            uint32_t vbo[2];
            glGenBuffers(1, vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glEnableVertexAttribArray(0);
            glBufferData(GL_ARRAY_BUFFER, k.mB * sizeof(glm::vec2), drawArray, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glEnable(GL_BLEND);
            glfwSetWindowUserPointer(window, this);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            double time, timeo;
            glfwSwapInterval(1);
            PaStream* stream;
            PaStreamParameters params;
            params.device = 21;
            params.channelCount = 1;
            params.sampleFormat = paFloat32;
            params.hostApiSpecificStreamInfo = nullptr;
            params.suggestedLatency = 0.5;
            
            
            err =  Pa_OpenStream(&stream, &params, nullptr, sampleRate, paFramesPerBufferUnspecified, 0, paCallback, this);
            if (err != paNoError)
                printf("error %i", err);
            Pa_StartStream(stream);
            while(!glfwWindowShouldClose(window))
            {
                timeo = time;
                time = glfwGetTime();
                CQT::transform(mAudioData, out, b, mAudioLength);
            
            
                max = 0.0f;
                for (unsigned int i = 0; i < k.mB; ++i) {
                    drawArray[2 * i + 0] = (float)i / k.mB * 2.0f - 1.0f;
                    float tmp = std::abs(out[i]);
                    drawArray[2 * i + 1] = tmp;
                    max = std::max(tmp, max);

                }
                for (unsigned int i = 0; i < k.mB; ++i) {
                    drawArray[2 * i + 1] = std::log(drawArray[2 * i +1]) * mScale + mOffset;
                }
                //printf("%f\n", drawArray[1]);
                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
                glBufferData(GL_ARRAY_BUFFER, k.mB * sizeof(glm::vec2), drawArray, GL_DYNAMIC_DRAW);
                p->use();
                glDrawArrays(GL_LINE_STRIP, 0, k.mB);
                for (unsigned int i = 0; i < b.getWidth(); ++i) {
                    debug[i].y = mAudioData[i] / 15.0;
                }
                glBufferData(GL_ARRAY_BUFFER, b.getWidth() * sizeof(glm::vec2), debug, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_LINE_STRIP, 0, b.getWidth());
               print->printfAt(-300.0f, 100.0f, 16.0f, 16.0f, u8"Fps:%03.3f", 1/(time-timeo));
                
                glfwSwapBuffers(window);
                glClear(GL_COLOR_BUFFER_BIT);
                glfwPollEvents();
                
            }
            Pa_StopStream(stream);
            Pa_CloseStream(stream);
            Pa_Terminate();

            std::cout << "Hello World. I'm Peach." << std::endl;

        }
    
        static int paCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
            Main* main = (Main*)userData;
            float* in = (float*)inputBuffer;
            //printf("Bufer:%u Offset:%u\n", framesPerBuffer, main->mDataOffset);
            for (unsigned int i = 0; i < framesPerBuffer; ++i) {
                main->mAudioData[main->mDataOffset++] = in[i];
                if (main->mDataOffset >= main->mAudioLength) {
                    main->mDataOffset = 0;
                }
            }
            return 0;
        
        
        }
        
    static void windowSizeCallback(GLFWwindow* win, int w, int h)
    {
        glViewport(0, 0, w, h);
        
        Main* main = (Main*)glfwGetWindowUserPointer(win);
        main->screen = Vec2<unsigned int>(w, h);
        main->print->setScreenSize(main->screen);
        
    }
    
    static void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
        Main* main = (Main*)glfwGetWindowUserPointer(win); 
        switch (key) {
            case GLFW_KEY_A:
                if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                    main->mOffset += 0.2;
                }
                break;
            case GLFW_KEY_Z:
                if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                    main->mOffset -= 0.2;
                }
                break;
            case GLFW_KEY_S:
                if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                    main->mScale *= 1.2;
                }
                break;
            case GLFW_KEY_X:
                if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                    main->mScale /= 1.2;
                }
                break;
        }
    }
    
    static void mouseCallback(GLFWwindow* win, double x, double y) {
        Main* main = (Main*)glfwGetWindowUserPointer(win);
        if (main->isClicked) {
            main->points.push_back(glm::vec2((x / main->screen.x - 0.5f) * 2.0f * main->screen.x / 1000.0f,
                            -(y / main->screen.y - 0.5f) * 2.0f * main->screen.y / 1000.0f));
        }
    }
    
    static void clickCallback(GLFWwindow* win, int button, int action, int mods) {
        Main* main = (Main*)glfwGetWindowUserPointer(win);
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_RELEASE)
            {
                main->points.clear();
            }
            main->isClicked = (action == GLFW_PRESS);
        }
    }
    
    static void APIENTRY debugCallback(unsigned int source, unsigned int type, GLuint id,
   GLenum severity, GLsizei length, const GLchar* message, const void* userData) {
        printf("%s\n", message);
    }
};


int main(){
    new Main();
}
