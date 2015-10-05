#include "Font.h"



void checkErr(const char *name)
{

    GLenum ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        printf(
            "ERROR: %X at %s\n",
            (ErrorCheckValue), name
        );
    }
}


Font::Font(unsigned int size, const char *name, unsigned int textureSize, TextureManager *textureManager) :
    _data(textureSize * textureSize * 0x100),
    //_glyphs(0x100),
    _size(size),
    _textureNumber(textureManager->getFreeTexture()),
    _name(name),
    m_freeType(size, name)
{

    unsigned int sizeSq = textureSize * textureSize;

    populateIndex();

    float *totalMap = (float*)malloc(sizeSq * m_charToIndex.size() * sizeof(float));
    _glyphs.resize(m_charToIndex.size());
    bool done = false;
    Hash h;

    h << size << textureSize << name;
    std::vector<uint32_t> hv = h.digest();

    std::stringstream str;
    str << std::setfill('0') << std::setw(8) << std::hex << hv[0] << hv[1] << hv[2] << hv[3];

    std::fstream tex(("res/textures/font" + str.str()).c_str(), std::fstream::in | std::fstream::binary);

    if ( !tex.fail())
    {

        std::vector<uint32_t> fv(4);
        tex.read((char*)fv.data(), sizeof(uint32_t) * 4);

        if (std::equal(hv.begin(), hv.end(), fv.begin()))
        {
            tex.read((char*)totalMap, sizeSq * m_charToIndex.size() * sizeof(float));
            done = true;
        }
    }

    tex.close();

    unsigned int cnt = 0, num = m_charToIndex.size();

    if ( !done )
    {
    	time_t start = time(nullptr);
        createGlyphsThreaded(&m_freeType, textureSize, totalMap);
        std::cout << "total Time:" << time(nullptr) - start << std::endl;




    }
    else
    {
        for (std::pair<unsigned int, unsigned int> i : m_charToIndex)
        {

        	createGlyph(i.second, i.first, &m_freeType, true);
        }
    }

    if ( !done )
    {
#ifdef __WIN32
        mkdir(u8"res/textures");
#else
	mkdir(u8"res/textures", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
        std::fstream texOut(("res/textures/font" + str.str()).c_str(), std::fstream::out | std::fstream::binary);
        texOut.write((char*)hv.data(), sizeof(uint32_t) * 4);
        texOut.write((char*)totalMap, sizeSq * m_charToIndex.size() * sizeof(float));


    }

    unsigned int nSize = m_charToIndex.size() % 4U;

    nSize = 4 - nSize + m_charToIndex.size();

    float *newMap = (float*)malloc(sizeSq * (nSize + 1) * sizeof(float));


    for (unsigned int i = 0; i <  m_charToIndex.size() / 4U; i++)
    {
    	for (unsigned int j = 0; j < sizeSq; j++)
    	{
            unsigned int nOff = i * sizeSq * 4 + j * 4;
            unsigned int oOff = i * sizeSq * 4 + j;
            newMap[nOff + 0] = totalMap[oOff + sizeSq * 0];
            newMap[nOff + 1] = totalMap[oOff + sizeSq * 1];
            newMap[nOff + 2] = totalMap[oOff + sizeSq * 2];
            newMap[nOff + 3] = totalMap[oOff + sizeSq * 3];
    	}

    }
    unsigned int iTmp = m_charToIndex.size();

    unsigned int tmpMod = m_charToIndex.size() % 4U;
    std::cout << tmpMod;
    if ( tmpMod != 0)
    {
    	unsigned int i = m_charToIndex.size() / 4U;
    	unsigned int tmpMod = m_charToIndex.size() % 4U;
    	for (unsigned int j = 0; j < sizeSq; j++)
            {
                unsigned int nOff = i * sizeSq * 4 + j * 4;
                unsigned int oOff = i * sizeSq * 4 + j;
                newMap[nOff + 0] = totalMap[oOff + sizeSq * 0];
                switch (tmpMod - 1)
                {
                case 3:
                    newMap[nOff + 3] = totalMap[oOff + sizeSq * 3];
                case 2:
                    newMap[nOff + 2] = totalMap[oOff + sizeSq * 2];
                case 1:
                    newMap[nOff + 1] = totalMap[oOff + sizeSq * 1];
                }
            }
    }


    unsigned int texture;

    glGenTextures(1, &texture );

    glActiveTexture( GL_TEXTURE0 + _textureNumber);

    glBindTexture( GL_TEXTURE_2D_ARRAY, texture );

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int tmp;

    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &tmp);

    std::cout << "Max array layers: " << tmp << std::endl;

    glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, textureSize, textureSize, nSize / 4, 0, GL_RGBA, GL_FLOAT, newMap);

    checkErr("test1");

    free(totalMap);
}

void Font::createGlyphsThreaded(FreeType *ft, unsigned int textureSize, float *totalMap)
{


    ThreadData td;
    td.it = m_charToIndex.begin();
    td.me = this;
    td.cnt = 0;
    td.name = _name;
    td.size = _size;
    td.textureSize = textureSize;
    td.dataOut = totalMap;
    
    unsigned int cpuCnt = std::thread::hardware_concurrency();

    std::thread* threads[cpuCnt];
    for (unsigned int i = 0; i < cpuCnt; i++)
    {
        threads[i] = new std::thread(&Font::threadFunc, &td);
        
#ifdef __WIN32
        uint64_t aff = (1 << i);
        SetThreadAffinityMask((void*)threads[i]->native_handle(), aff);
#endif
    }


    for (unsigned int i = 0;i < cpuCnt; i++)
    {
        threads[i]->join();
    }


}

uint16_t Font::threadFunc(void* lpParam )
{
    ThreadData *td = (ThreadData*)lpParam;
    //unsigned int i;

    FreeType ft(td->size, td->name);
    std::unordered_map<unsigned int, unsigned int>::iterator i;
    
    td->me->ctSec.lock();
    i = td->it++;
    std::cout << "I am char " << i->first << " with id " << i->second << std::endl;
    td->me->ctSec.unlock();
    
    while ( i != td->me->m_charToIndex.end())
    {

    	FT_GlyphSlot gl = td->me->createGlyph(i->second, i->first, &ft, true);
    	td->me->createTexFromGlyph(td->textureSize, gl, td->dataOut, i->second);
        td->me->ctSec.lock();
        if (td->it == td->me->m_charToIndex.end())
        {
                td->me->ctSec.unlock();
        	break;
        }
        i = td->it++;
		std::cout << "I am char " << i->first << " with id " << i->second << std::endl;
		td->me->ctSec.unlock();
    }
    return 0;
}

FT_GlyphSlot Font::createGlyph(unsigned int id, unsigned int c, FreeType *ft, bool render)
{

	FT_GlyphSlot gl;
	if ( render )
		gl = ft->createChar(c);
	else
		gl = ft->createCharNoRender(c);
    _glyphs[id].advance = Vec2<float>(gl->advance.x >> 6, gl->advance.y >> 6);
    _glyphs[id].size = Vec2<float>(gl->bitmap.width, gl->bitmap.rows);
    _glyphs[id].offset = Vec2<float>(gl->bitmap_left, gl->bitmap_top) + ( Vec2<float>(_size) - _glyphs[id].size ) / Vec2<float>(-2, 2);
    return gl;
}

void Font::createTexFromGlyph(unsigned int textureSize, FT_GlyphSlot gl, float *totalMap, unsigned int id)
{
    DistMap dm(gl->bitmap.buffer, _size, gl->bitmap.width, gl->bitmap.rows);

    float *out = dm.generate(textureSize);

    unsigned int sizeSq = textureSize * textureSize;


    float min = out[0];
    float max = out[0];

    for (unsigned int j = 0; j < sizeSq; j++)
    {
        if ( out[j] > max )
            max = out[j];
        if ( out[j] < min )
            min = out[j];
    }

    if (max == min )
    {
        for (unsigned int j = 0; j< sizeSq;j++)
        {
            out[j] = 0.0f;
        }
    }
    else
    {


        float diff = 1/ ( max - min );

        for (unsigned int j = 0; j< sizeSq;j++)
        {
            out[j] = out[j] * diff + .5f;
        }
    }

    memcpy(totalMap + sizeSq * id, out, sizeSq * sizeof(float));

    delete out;
}

void Font::populateIndex()
{
    FT_ULong charCode;
    FT_UInt gIndex;
    unsigned int id = 0;
    charCode = m_freeType.getFirstChar(&gIndex);
    while ( gIndex != 0 )
    {
        m_charToIndex.insert(std::pair<unsigned int, unsigned int>(charCode, id));
        id++;

        charCode = m_freeType.getNextChar(charCode, &gIndex);
    }
}

unsigned int Font::getIdFromChar(unsigned int c)
{
    try {
        return m_charToIndex.at(c);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

Font::~Font()
{
    //dtor
}
