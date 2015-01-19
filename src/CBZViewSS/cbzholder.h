#ifndef CBZHOLDER_H
#define CBZHOLDER_H

#include "cbholder.h"
#include "unzip.h"

#ifdef GCWZERO
extern int currentpagenumber;
#endif


class CBZHolder : public CBHolder
{
    public:
        CBZHolder();
        virtual ~CBZHolder();
        virtual bool InitFile(const char* filename);
        virtual bool GetFirstPage();
        virtual bool GetNextPage();
        virtual bool GetPrevPage();
    protected:
        virtual bool EnumerateFiles();
        virtual bool CloseFile();
        bool ReadFile(archive_data &data);
    private:
        unzFile m_cbzFile;



};

#endif // CBZHOLDER_H
