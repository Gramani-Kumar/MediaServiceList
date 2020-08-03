#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>


typedef struct _contentIds_
{

    int  contentId;
    int  totalContent;
    int  maxContent;
    int  folderId;

}contentIds;


static int clmScanDir(char *path, contentIds *content);


int main()
{
   char folder[512] = {0};
   contentIds  content;

   printf("Enter Folder to Scan \n");
   scanf("%s", folder);

   //Root Directory.
   
   content.contentId    =  0;
   content.folderId     = -1;   //Parent Id.
   content.maxContent   = 300;
   content.totalContent =  0;
   

   //Insert into DB.

   content.contentId    =  1000;
   clmScanDir(folder, &content);

   printf("Total Contents %d \n", content.totalContent);

   return 0;
}


int clmScanDir(char *p_shareFolder, contentIds *p_content)
{
    struct dirent	**namelist = NULL;
    int 		    retValue = 0;
    int 		    numOfEntries = 0;
    int 		    index = 0;
    struct stat		fileStat;
    char		    filePath[1024] = {0};
    int 		    folderId = 0;
    int             childCount = 0;
    
    if(p_shareFolder == NULL || p_content == NULL)
    {
        return -1;
    }
     
    folderId = p_content->contentId;
    numOfEntries = scandir(p_shareFolder, &namelist, NULL, NULL);
    if(numOfEntries < 0)
    {
        printf("Error occured for scanning directory %s\n", p_shareFolder); 
        perror("");
        return -2;
    }

    index = -1;
    while(index < numOfEntries -1)
    {
       index ++;
      
       if(strcmp( namelist[index]->d_name, ".") == 0 || strcmp( namelist[index]->d_name, "..") == 0)
       {
           free(namelist[index]);
           continue;
       }
       
       //File Path
       sprintf(filePath,"%s/%s",p_shareFolder, namelist[index]->d_name);
       
       retValue = lstat(filePath, &fileStat);
       if(retValue < 0)
       {
           free(namelist[index]);
           printf("Not Sure why it is not able to access \n");
           perror("");
           continue;
       }

       //Verify If max Content reaches
       if(p_content->totalContent == p_content->maxContent)
       {
           //Free NameList;
           while(index < numOfEntries)
           {
               free(namelist[index]);
               index ++;
           }
           break;
       }

       //Entry
       p_content->totalContent ++;
       childCount ++;

       if(S_ISREG(fileStat.st_mode) )
       {
           p_content->contentId ++;

           printf("[%3d] The File Name :: %s \n", p_content->contentId, namelist[index]->d_name);
           free(namelist[index]);
       }
       else if(S_ISDIR(fileStat.st_mode))
       {
           p_content->contentId ++;

           printf("[%3d] :: [%2d] The Dir  Name :: %s \n", p_content->contentId, folderId, namelist[index]->d_name);
           clmScanDir(filePath, p_content);
           free(namelist[index]);
       }
       else
       {
           p_content->totalContent --;
           childCount --;
           free(namelist[index]);
           printf("Content Neither Directory nor Regular File [%s]\n", filePath);
           continue;
       }

    }

    free(namelist);

    //Update Child Count into DB
    printf("The Directory [%d] contains [%d] Childs\n", folderId, childCount);
    
    return 0;
}
