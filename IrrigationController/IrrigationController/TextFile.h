#ifndef __TEXT_FILE_H_
#define __TEXT_FILE_H_




#include <MySD.h>
#include "Common.h"
#include "CString.h"




const uint8_t READ = O_READ;
const uint8_t READ_APPEND = (O_READ | O_WRITE | O_CREAT);
const uint8_t APPEND = (O_WRITE | O_CREAT);
const uint8_t REWRITE = (O_WRITE | O_CREAT | O_TRUNC);
const uint8_t CREATE = (O_WRITE | O_CREAT);




class CTextFile
{
  public:
    // Construction, destruction and initialisation
    CTextFile();
    CTextFile(const char* strFilename, const uint8_t nMode);
    virtual ~CTextFile();

    bool readWord(CString& strWord);
    
    bool readWord(CString& strWord, const char cDelim);
        
    bool open(const char* strFilename, const uint8_t nMode);
#ifndef __SAM3X8E__
    bool open(const __FlashStringHelper* strFilename, const uint8_t nMode);
#endif
    uint32_t position()
    {
      return m_file.position();
    }
    void seek(uint32_t nFilePos)
    {
      m_file.seek(nFilePos);
    }
    char peek()
    {
      return m_file.peek();
    }
    char readChar()
    {
      return m_file.read();
    };
    bool readLine(CString& strLine, const bool bIncludeEOL = false, const bool bDebug = false);
    bool read(CString &strData);
    bool read(char* arrayBuff, const uint16_t nMaxLen)
    {
      return m_file.read(arrayBuff, nMaxLen);
    }
    size_t write(const uint8_t *arrayBuff, size_t nBuffSize)
    {
      return m_file.write(arrayBuff, nBuffSize);
    };
    void write(const char* strText);
#ifndef __SAM3X8E__
    void write(const __FlashStringHelper* strText);
#endif
    void write(const uint32_t nNum)
    {
      write(fromUint(nNum, 10));
    }
    void write(const uint16_t nNum)
    {
      write(fromUint(nNum, 10));
    }
    void write(const uint8_t nNum)
    {
      write(fromUint(nNum, 10));
    }
    void write(const int32_t nNum)
    {
      write(fromUint(nNum, 10));
    }
    void write(const int16_t nNum)
    {
      write(fromUint(nNum, 10));
    }
    void write(const int8_t nNum)
    {
      write(fromUint(nNum, 10));
    }
    void write(const float fNum, const uint8_t nDecPlaces = 2)
    {
      write(fromReal((double)fNum, nDecPlaces));
    }
    void write(const double dNum, const uint8_t nDecPlaces = 2)
    {
      write(fromReal(dNum, nDecPlaces));
    }
    void write(const char cCh);
    void writeLine(const char* strText);
#ifndef __SAM3X8E__
    void writeLine(const __FlashStringHelper* strText);
#endif
    void writeLine(const uint32_t nNum)
    {
      writeLine(fromUint(nNum, 10));
    }
    void writeLine(const uint16_t nNum)
    {
      writeLine(fromUint(nNum, 10));
    }
    void writeLine(const uint8_t nNum)
    {
      writeLine(fromUint(nNum, 10));
    }
    void writeLine(const int32_t nNum)
    {
      writeLine(fromUint(nNum, 10));
    }
    void writeLine(const int16_t nNum)
    {
      writeLine(fromUint(nNum, 10));
    }
    void writeLine(const int8_t nNum)
    {
      writeLine(fromUint(nNum, 10));
    }
    void writeLine(const float fNum, const uint8_t nDecPlaces = 2)
    {
      writeLine(fromReal((double)fNum, nDecPlaces));
    }
    void writeLine(const double dNum, const uint8_t nDecPlaces = 2)
    {
      writeLine(fromReal(dNum, nDecPlaces));
    }
    void writeln(const char cCh);
    
    bool eof();
    uint16_t available()
    {
      return m_file.available();
    };
    
    void close()
    {
      m_file.close();
    };

    operator File&()
    {
      return m_file;
    };
    
    operator bool()
    {
      return m_file;
    };

    bool seekRel(int32_t nPos);
    uint32_t size()
    {
      return m_file.size();
    }

    void dump(const char* strFilename);

    char* filename()
    {
      return m_file.name();
    };
    
  protected:
    File m_file;
    static const char* m_strEOL;
};

#endif




