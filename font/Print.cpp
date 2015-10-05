

#include "Print.h"

Print::Print(Font *font) :
    _mat(IDENTITY_MATRIX),
	_font(font)
{

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(2, _vbo);
    Shader s("res/shader/fontVertex.c", true, GL_VERTEX_SHADER);
    _program.attach(s);
    Shader s2("res/shader/fontFragment.c", true, GL_FRAGMENT_SHADER);
    _program.attach(s2);

    _program.build();

    glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);


    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);



    glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 0, 0);

    glEnableVertexAttribArray(1);
    
    m_stepSize = _program.getUniformLocation("stepSize");
    m_tex = _program.getUniformLocation("tex");
    m_charMat = _program.getUniformLocation("charMat");

}

Print::~Print() {
	// TODO Auto-generated destructor stub
}

//void Print::at(Vec2<float> pos, )


void Print::printfAt(float x, float y, float sx, float sy, const char *fmt, ...)
{
	unsigned char *text = new unsigned char[256];

	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf((char*)text, fmt, ap);
	va_end(ap);

	unsigned int k = strlen((char*)text);

	uint32_t *data = new uint32_t[k * 6];

	struct point {
        float x;
        float y;
        float s;
        float t;
    };


	point *points = new point[k * 6];

	Vec2<float> adv(0.0f, 0.0f);

	unsigned int cCnt = 0;
	//unsigned int ch;
	unsigned char *textO = text;
    while (true)
    {
    	uint32_t ch;
    	text += getUcs4CharFromString(&ch, text);
    	if (ch == 0)
    		break;

        if (ch == '\n')
        {
            adv = adv * Vec2<float>(0, 1) + Vec2<float>(0, -1);
        }
        else
        {
        	unsigned int key = _font->getIdFromChar(ch);

            data[cCnt * 6] =
                data[cCnt * 6 + 1] =
                data[cCnt * 6 + 2] =
                data[cCnt * 6 + 3] =
                data[cCnt * 6 + 4] =
                data[cCnt * 6 + 5] = key;

            Vec2<float>off = _font->_glyphs[key].offset / (float)_font->_size * Vec2<float>(1,1) + adv;


            points[cCnt * 6 + 0] = (point){off.x, off.y, 0.0f, 1.0f};
            points[cCnt * 6 + 1] = (point){off.x, off.y + 1, 0.0f, 0.0f};
            points[cCnt * 6 + 2] = (point){off.x + 1, off.y + 1, 1.0f, 0.0f};

            points[cCnt * 6 + 3] = (point){off.x, off.y, 0.0f, 1.0f};
            points[cCnt * 6 + 4] = (point){off.x + 1, off.y + 1, 1.0f, 0.0f};
            points[cCnt * 6 + 5] = (point){off.x + 1, off.y, 1.0f, 1.0f};
            Vec2<float> test = _font->_glyphs[key].advance;
            adv += _font->_glyphs[key].advance / _font->_size;
            cCnt++;
        }
    }

    
    glBindVertexArray(_vao);
    _program.use();
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, cCnt * 6 * sizeof(point), points, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, cCnt * 6 * sizeof(uint32_t), data, GL_DYNAMIC_DRAW);


    Matrix<float> tmpmat(IDENTITY_MATRIX);// = _mat;

    tmpmat.scale(sx, sy, 1.0f);

    tmpmat.translate(x, y, 0.0f);

    tmpmat *= _mat;

    glUniform1f(m_stepSize, 2 / ( sx) );

    glUniform1i(m_tex, _font->_textureNumber);

    glUniformMatrix4fv(m_charMat, 1,  GL_FALSE, tmpmat.data.data());
    glDrawArrays(GL_TRIANGLES, 0, cCnt * 6);

    delete[] points;
    delete[] data;
    delete[] textO;
    //std::cout << "bla" << std::endl;

}

unsigned int Print::getUcs4CharFromString(uint32_t *out, unsigned char *in)
{ //FIXME can overflow in buffer if last char contains malformed unicode!
	unsigned int btCount = 0;
	unsigned char in1 = *((unsigned char*)in);
	//uint32_t tmp = 0;

	if (in1 < 0b11000000)
	{
		*out = in1;
		return 1;
	}
	else if (in1 < 0b11100000)
	{
		*out = (*(in + 1) & 0b00111111) | ((*(in) & 0b00011111) << 6);
		return 2;
	}
	else if (in1 < 0b11110000)
	{
		*out = (*(in + 2) & 0b00111111) | ((*(in + 1) & 0b00111111) << 6) | ((*(in) & 0b00001111) << 12);
		return 3;
	}
	else if (in1 < 0b11111000)
	{
		*out = (*(in + 3) & 0b00111111) | ((*(in + 2) & 0b00111111) << 6) | ((*(in + 1) & 0b00111111) << 12) | ((*(in) & 0b00000111) << 18);
		return 4;
	}




	return 1;
}

void Print::setScreenSize(Vec2<unsigned int> size)
{
	_screen = size;
    float ratio = (float)size.x / (float)size.y;
    _mat.set(IDENTITY_MATRIX);
    _mat.ortho(-(float)size.x/2, (float)size.x/2, -(float)size.y/2, (float)size.y/2, 1.0f, -1.0f);
}
