#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <io.h>
#elif __linux__
#include <inttypes.h>
#include <unistd.h>
#define __int64 int64_t
#define _close close
#define _read read
#define _lseek64 lseek64
#define _O_RDONLY O_RDONLY
#define _open open
#define _lseeki64 lseek64
#define _lseek lseek
#define stricmp strcasecmp
#endif
#include <libgen.h>
#include <string.h>
#include <sys/time.h>
#include <dirent.h>

#ifdef WIN32
#include <conio.h>

#ifndef PATH_MAX
#define PATH_MAX _MAX_PATH
#endif
#define realpath(N, R) _fullpath((R), (N), PATH_MAX)
#endif

int try_filename(char *path)
{
  char *dot_pos = strrchr(path, '.');
  int ext_len = 0;
  char ext[ext_len];
  if (dot_pos == NULL)
  {
    dot_pos = &path[strlen(path)];
    printf("dot_pos %s\n", dot_pos);
    ext[0] = '\0';
  }
  else
  {
    ext_len = strlen(dot_pos);
    strcpy(ext, dot_pos);
  }

  int count = 1;
  char num[10];
  do
  {
    sprintf(num, "%d", count);
    strcpy(dot_pos, " (");
    strcat(dot_pos, num);
    strcat(dot_pos, ")");
    strcat(dot_pos, ext);
    count++;
  } while (access(path, 0) == 0);
  return 0;
}

int copy_file(const char *input, const char *output)
{
  printf("创建文件 \"%s\"\n", output);
  char *buffer = (char *)malloc(BUFSIZ);
  FILE *fpInput = fopen(input, "rb");
  FILE *fpOutput = fopen(output, "wb");
  int readCount; // 实际读取的字节数
  while ((readCount = fread(buffer, 1, BUFSIZ, fpInput)) > 0)
  {
    fwrite(buffer, readCount, 1, fpOutput);
  }
  fclose(fpInput);
  int r;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  unsigned now = (unsigned)((tv.tv_sec - 1734796800) * 1000) + (tv.tv_usec / 1000);
  srand(now);

  r = rand() % 10 + 48;
  buffer[0] = r;
  buffer[1] = '\0';
  fwrite(buffer, 1, 1, fpOutput);
  fclose(fpOutput);
  return 0;
}

int main(int argc, char *argv[])
{
  if (argc >= 2)
  {
    if (access(argv[1], 4))
    {
      printf("文件 \"%s\" 不存在\n", argv[1]);
    }
    else
    {
      char program_path[PATH_MAX];
      realpath(argv[0], program_path);
      // for (int i = 0; i < strlen(path); i++)
      // {
      //   if (path[i] == '\\')
      //   {
      //     path[i] = '/';
      //   }
      // }
      dirname(program_path);
      // printf("program_path: %s\n", program_path);

      char *path = argv[1];
      int last = 0;
      for (int i = 0; i < strlen(path); i++)
      {
        if (path[i] == '/' || path[i] == '\\')
        {
          last = i;
        }
      }

      char filename[strlen(path)];
      if (last == 0)
      {
        strcpy(filename, path);
      }
      else
      {
        strcpy(filename, path + last + 1);
      }
      // // 盘符大写
      // if (strchr(path, ':') != NULL && path[0] > 96)
      // {
      //   path[0] = path[0] - 32;
      // }
      // dirname(argv[0]);

      unsigned long program_path_len = strlen(program_path);
      unsigned long filename_len = strlen(filename);
      char filepath[program_path_len + filename_len + 14];
      strcpy(filepath, program_path);
      strcat(filepath, "/");
      strcat(filepath, filename);
      // printf("filepath: %s\n", filepath);

      if (access(filepath, 0) != 0)
      {
        copy_file(argv[1], filepath);
      }
      else
      {
        try_filename(filepath);
        copy_file(argv[1], filepath);
      }
    }
  }
  else
  {
    printf("未拖入文件\n");
  }
  char ch;
#ifdef WIN32
  printf("按任意键退出...");
  getch();
#endif
  return 0;
}
