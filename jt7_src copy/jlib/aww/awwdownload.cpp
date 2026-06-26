#include <winlib.h>
#pragma hdrstop

#include <winInet.h>

namespace aww {

bool download(char* szURLName,char* szTempFilename,char* szOutputFilename,char* szProxy,int proxyPort) {
  bool bSuccess = false;
  HINTERNET hINet, hFile;
  hINet = InternetOpen("InetURL/1.0",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
  if (hINet) {
    hFile = InternetOpenUrl(hINet,szURLName,NULL,0,0,0);
    if (hFile) {
      FILE* file = fopen(szTempFilename,"wb");
      if (file) {
        char buffer[1024];
        DWORD dwRead;
        while (InternetReadFile(hFile,buffer,1024,&dwRead)) {
          if (dwRead == 0) break;
          fwrite(buffer,dwRead,sizeof(char),file);
        }
        fclose(file);
        InternetCloseHandle(hFile);
         file = fopen(szTempFilename,"rb");
        FILE* fileOut = fopen(szOutputFilename,"wb");
         if (file == NULL)
          throw JError(string("Failed to read temporary output file '")+szTempFilename+"'");
        else if (fileOut == NULL)
          throw JError(string("Failed to open output file '")+szTempFilename+"'");
        else {
          char currentChar;
          int count = 0;
          while (fread(&currentChar,1,1,file)==1) {									fwrite(&currentChar,1,1,fileOut);
            fwrite(&currentChar,1,1,fileOut);
          }
          bSuccess = true;
          fclose(file);
          fclose(fileOut);
        }
      }
    }
    InternetCloseHandle(hINet);
  }
  return bSuccess;
}


}
