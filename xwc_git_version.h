#ifndef XWC_VERSION_H
#define XWC_VERSION_H

#define XWC_BRANCH "main"
#define XWC_CURRENT_SHA1 "bc693abae31d950ccdd646c6aa1fd46f227f4c08"
#define XWC_REPO_MODIFIED  1


#if XWC_REPO_MODIFIED == 1
  #ifndef GITHEAD_NOWARNING
    #pragma message ("******************************************")
    #pragma message ("*                                        *")
    #pragma message ("*       WARNING: repository modified     *")
    #pragma message ("*        changes not committed for       *")
    #pragma message ("*                   XWC                 *")
    #pragma message ("*                                        *")
    #pragma message ("******************************************")
  #endif
#endif


#endif
