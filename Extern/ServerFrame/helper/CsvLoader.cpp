#include "CsvLoader.h"
#include "KxCore.h"



CsvLoader::CsvLoader()
{
    m_nDocOffset = 0;
    m_nDocStep = 0;
    m_nLineOffset = 0;
    m_nLineStep = 0;
}

CsvLoader::~CsvLoader()
{
}

void CsvLoader::Release()
{
    m_nDocOffset = 0;
    m_nDocStep = 0;
    m_nLineOffset = 0;
    m_nLineStep = 0;
    m_CurLine = "";
    m_Document = "";
}

bool CsvLoader::LoadCSV(const char* fileName)
{
    if (NULL == fileName)
    {
        return false;
    }

    m_Document = getStringFromFile(fileName);
    m_nDocOffset = m_nDocStep = 0;

    //Èç¹ûÎÄ¼þÎª¿ÕÒ²»á·µ»ØÊ§°Ü
    return NextLine();
}

// NextLine»áÌø×ªÖÁÏÂÒ»ÐÐ
// ·µ»Øfalse±íÊ¾ÒÑ¾­µ½ÁËÎ²²¿£¬·µ»Øtrue±íÊ¾¶Áµ½ÁËÐÂµÄÒ»ÐÐÊý¾Ý
bool CsvLoader::NextLine()
{
skipline:
    // »»ÐÐÕâÀïÐèÒª¿¼ÂÇ\r\nµÄÎÊÌâ
    m_nDocStep = m_Document.find_first_of('\n', m_nDocOffset);
    if (std::string::npos == m_nDocStep)
    {
        // ÕâÀï»¹ÐèÒª¿¼ÂÇ×îºóÒ»ÐÐÕÒ²»µ½\nµÄÇé¿ö
        if (m_nDocOffset < m_Document.size())
        {
            // ½«×îºóÊ£ÓàµÄÒ»ÐÐÄÚÈÝÈ¡³ö
            m_CurLine = m_Document.substr(m_nDocOffset, m_Document.size() - m_nDocOffset);
            m_nDocOffset = m_Document.size();
            return true;
         }
        return false;
    }

    if (m_Document.at(m_nDocStep - 1) == '\r')
    {
        m_CurLine = m_Document.substr(m_nDocOffset, m_nDocStep - m_nDocOffset - 1);
    }
    else
    {
        m_CurLine = m_Document.substr(m_nDocOffset, m_nDocStep - m_nDocOffset);
    }
    m_nLineOffset = 0;
    m_nDocOffset = m_nDocStep + 1;

    if (isCurrentLineNull())
    {
        goto skipline;
    }
    else
    {
        return true;
    }
}

std::string CsvLoader::LeftStr()
{
    return m_CurLine.substr(m_nLineOffset, m_CurLine.size() - m_nLineOffset);;
}

// ½«µ±Ç°ÐÐÏÂÒ»¸ö×Ö¶Î½âÎöÎªstring²¢·µ»Ø
// Èç¹û½âÎöÊ§°Ü·µ»Ø""
std::string CsvLoader::NextStr()
{
    m_nLineStep = m_CurLine.find_first_of(',', m_nLineOffset);
    std::string seg = m_CurLine.substr(m_nLineOffset, m_nLineStep - m_nLineOffset);
    m_nLineOffset = m_nLineStep + 1;
    return Trim(seg);
}

// ½«µ±Ç°ÐÐÏÂÒ»¸ö×Ö¶Î½âÎöÎªint²¢·µ»Ø
// Èç¹û½âÎöÊ§°Ü·µ»Ø0
int CsvLoader::NextInt()
{
    return atoi(NextStr().c_str());
}

// ½«µ±Ç°ÐÐÏÂÒ»¸ö×Ö¶Î½âÎöÎªfloat²¢·µ»Ø
// Èç¹û½âÎöÊ§°Ü·µ»Ø0.0f
float CsvLoader::NextFloat()
{
    return static_cast<float>(atof(NextStr().c_str()));
}

std::string CsvLoader::Trim(std::string str)
{
    if (str.empty())
    {
        return str;
    }
    size_t posL = str.find_first_not_of(' ');
    size_t posR = str.find_last_not_of(' ');
    if ((posL == std::string::npos && posR == std::string::npos)
        || (0 == posL && (str.size() - 1) == posR))
    {
        return str;
    }
    std::string ret = str.substr(posL, posR - posL + 1);
    return ret;
}

bool CsvLoader::isCurrentLineNull()
{
    for (size_t i = 0; i < m_CurLine.length(); ++i)
    {
        switch (m_CurLine.c_str()[i])
        {
        case ' ':
        case ',':
        case '\n':
        case '\r':
            break;

        default:
            return false;
            break;
        }
    }
    return true;
}

// ½«Ò»¸ö×Ö·û´®ÒÔ·Ö¸ô·û sep ·Ö¸ô¿ª£¬×ª»»Îª×Ö·û´®£¬ÌîÈëvectorÈÝÆ÷ÖÐ
bool CsvLoader::SplitStrToVector(const std::string &str, char sep, std::vector<std::string>& out)
{
    int pos = 0;
    int step = 0;

    while (static_cast<unsigned int>(pos) < str.length() && step != -1)
    {
        step = static_cast<int>(str.find_first_of(sep, pos));
        std::string seg = str.substr(pos, step);
        out.push_back(seg);
        pos = step + 1;
    }

    return out.size() > 0;
}

// ½«Ò»¸ö×Ö·û´®ÒÔ·Ö¸ô·û sep ·Ö¸ô¿ª£¬×ª»»Îªfloat£¬ÌîÈëvectorÈÝÆ÷ÖÐ
bool CsvLoader::SplitFloatToVector(const std::string &str, char sep, std::vector<float>& out)
{
    int pos = 0;
    int step = 0;

    while (static_cast<unsigned int>(pos) < str.length() && step != -1)
    {
        step = static_cast<int>(str.find_first_of(sep, pos));
        std::string seg = str.substr(pos, step);
        float value = static_cast<float>(atof(seg.c_str()));
        out.push_back(value);
        pos = step + 1;
    }

    return out.size() > 0;
}

// ½«Ò»¸ö×Ö·û´®ÒÔ·Ö¸ô·û sep ·Ö¸ô¿ª£¬×ª»»Îªint£¬ÌîÈëvectorÈÝÆ÷ÖÐ
bool CsvLoader::SplitIntToVector(const std::string &str, char sep, std::vector<int>& out)
{
    int pos = 0;
    int step = 0;

    while (static_cast<unsigned int>(pos) < str.length() && step != -1)
    {
        step = static_cast<int>(str.find_first_of(sep, pos));
        std::string seg = str.substr(pos, step);
        int value = atoi(seg.c_str());
        out.push_back(value);
        pos = step + 1;
    }

    return out.size() > 0;
}

std::string CsvLoader::getStringFromFile(const std::string &path)
{
    //FILE *fp = fopen(path.c_str(), "rb");
    FILE *fp = fopen(path.c_str(), "rt");
    if (NULL == fp)
    {
        return "";
    }

    char* buffer = NULL;
    int size = 0;
    int readsize = 0;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp) + 1;
    fseek(fp, 0, SEEK_SET);

    buffer = (char*)malloc(size);
    memset(buffer, 0, size);

    readsize = fread(buffer, sizeof(unsigned char), size, fp);
    fclose(fp);

    std::string ret(buffer, readsize);
    free(buffer);
    return ret;
}


