#include <cstdio>
#include <iostream>
#include "cbzholder.h"
#include "unzip.h"

using namespace std;
#ifdef GCWZERO
int currentpagenumber;
#endif


CBZHolder::CBZHolder():CBHolder()
{
    //ctor
}

CBZHolder::~CBZHolder()
{
    DeleteCurrentPage();
    CloseFile();

}

bool CBZHolder::InitFile(const char* filename)
{
    bool bRes=false;
    m_cbzFile = unzOpen OF((filename));
    if (m_cbzFile)
    {
        bRes = EnumerateFiles();
    }

    return bRes;
}
bool CBZHolder::GetFirstPage()
{
    bool bRes = false;
    m_nPage = 0;
#ifdef GCWZERO
    currentpagenumber=m_nPage+1; //page 1 = m_nPage 0
#endif

    if ( m_bInited )
    {
        string strName;
        if (vecFiles.size())
        {
           bRes = ReadFile(vecFiles.at(m_nPage));
        }

    }

    return bRes;
}

bool CBZHolder::ReadFile(archive_data& data)
{
    bool bRes = false;

    if (UNZ_OK == unzLocateFile OF((m_cbzFile,data.strFile.c_str(),0)))
            {
               if (UNZ_OK == unzOpenCurrentFile OF((m_cbzFile)))
               {


                       Uint8 *pFileData=(Uint8*)malloc(data.nUncompressedSize);
                       #ifdef GCWZERO
                       #define BUFFER_SIZE 8192
                       #else
                       #define BUFFER_SIZE 4096
                       #endif
                       Uint8 data_buffer[BUFFER_SIZE];
                       size_t nPointer=0;
                       size_t nRes=unzReadCurrentFile OF((m_cbzFile,data_buffer,BUFFER_SIZE));
                       while (nRes>0)
                       {
                           memcpy(&(pFileData[nPointer]),data_buffer,nRes);
                           nPointer = nPointer + nRes;
                           nRes=unzReadCurrentFile OF((m_cbzFile,data_buffer,BUFFER_SIZE));
                           cout<<nRes<<" bytes read..."<<endl;
                       }
                       if ( nPointer == data.nUncompressedSize  )
                       {
                          m_pCurrentFile = pFileData;
                          bRes=true;
                       }
                       else
                       {
                           free(pFileData);
                           cerr<<"Error reading archived file"<<endl;
                       }

               }
               else
               {
                   cerr <<"Unable to open file "<<data.strFile<<" inside archive"<<endl;
               }
            }
            else
            {
                cerr <<"Unable to locate file "<<data.strFile<<endl;
            }

  return bRes;

}

bool CBZHolder::GetNextPage()
{
    if (m_nPage >= vecFiles.size())
     return false;
    DeleteCurrentPage();
    m_nPage++;
#ifdef GCWZERO
    currentpagenumber++;
#endif

    bool bRes = false;

    if ( m_bInited )
    {
        if (vecFiles.size() > (size_t)m_nPage)
        {
            bRes = ReadFile(vecFiles.at(m_nPage));
        }

    }

    return bRes;

}

bool CBZHolder::GetPrevPage()
{
    bool bRes = false;
    if (m_nPage == 0 )
    {
        return false;
    }

     DeleteCurrentPage();
     m_nPage--;
#ifdef GCWZERO
    currentpagenumber--;
#endif


    if ( m_bInited )
    {

        if (vecFiles.size() > (size_t)m_nPage)
        {
            bRes = ReadFile(vecFiles.at(m_nPage));
        }

    }

    return bRes;

}


bool CBZHolder::EnumerateFiles()
{
    bool bRes = false;
    if(m_cbzFile)
    {
        int nRes = unzGoToFirstFile OF((m_cbzFile));
        if(nRes == UNZ_OK )
        {
            char file[BUFFER_SIZE];


              while ( nRes == UNZ_OK )
              {
                  unz_file_info fi;
                  int nResData = unzGetCurrentFileInfo OF((m_cbzFile,&fi, file, BUFFER_SIZE,NULL,NULL,NULL,NULL));
                  if (nResData == UNZ_OK)
                  {
                      archive_data data;
                      data.strFile=file;
                      data.nUncompressedSize=fi.uncompressed_size;
                      bool bIsDSSTORE = (data.strFile.find(".DS_Store") != string::npos); //not mac ds store
                      bool bIsTHUMBNAIL = (data.strFile.find(".db") != string::npos);
                      if ( (data.strFile[data.strFile.length()-1] != '/') && (!bIsDSSTORE) && (!bIsTHUMBNAIL)){ //not a dir ot thumbnail
                      vecFiles.push_back(data);
                      cout << "Read file "<<file<<endl;
                      }
                  }
                  nRes = unzGoToNextFile OF((m_cbzFile));
              }

              if (nRes == UNZ_END_OF_LIST_OF_FILE)
              {
                  bRes = true;
                  m_bInited = true;
              }
              else
              {
                  cerr <<"Unable to read through archive "<<endl;
              }

        }
        else
        {
            cerr <<"Unable to read archive "<<endl;
        }
    }

    return bRes;

}
bool CBZHolder::CloseFile()
{
    DeleteCurrentPage();
    if (m_cbzFile)
     unzClose OF((m_cbzFile));

    return true;

}
