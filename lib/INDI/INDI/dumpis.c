#ifndef __DUMPIS_C__
/* This file is #include'd into indiserver.c */

static FILE* STDDUMPIS = NULL;
static char* dumpts;
#define DUMPIS1(INDENT,FPF) fprintf(STDDUMPIS, "%s: %s", dumpts, INDENT); FPF; fprintf(STDDUMPIS, "%s", "\n")
#define BL2TXT(BLOBH) ( B_NEVER==(BLOBH) ? "B_NEVER" \
                      : (B_ALSO==(BLOBH) ? "B_ALSO" \
                      : (B_ONLY==(BLOBH) ? "B_ONLY" : "<unknown>")) \
                      )

char* dumpescape(char* praw)
{
    static char empty = '\0';
    static char* pc = &empty;
    static int pclen = 0;
    char* pin;
    char* pout;
    int newlen = 0;

    for (pin=praw; *pin; ++pin)
    {
        switch(*pin)
        {
        case '"': case '\n': ++newlen;
        default: ++newlen; break;
        }
    }

    if (newlen > pclen)
    {
        if (pc != &empty) { free(pc); }
        pc = (char*) malloc(newlen+1);
    }

    pout = pc;
    for (pin=praw; *pin; ++pin)
    {
        switch(*pin)
        {
        case '"': case '\n': *(pout++) = '\\';
        default: *(pout++) = *pin=='"' ? *pin : *pin=='\n' ? 'n' : *pin;
        }
    }
    *pout = '\0';

    return pc;
}

void dumpMsg(Msg* mp)
{
    fprintf(STDDUMPIS, "{ \"%s\": %d", "count", mp->count);
    fprintf(STDDUMPIS, ", \"%s\": %ld", "content_length", mp->cl);
    fprintf(STDDUMPIS, ", \"%s\": \"%s\"", "content", mp->cp ? dumpescape(mp->cp) : "<null>");
    fprintf(STDDUMPIS, ", \"%s\": \"%s\"", "malloced", mp->cp!=mp->buf ? "True" : "False" );
    fprintf(STDDUMPIS, "}");
}
void dumpProperty(Property* prop)
{
    fprintf(STDDUMPIS, "{ \"%s\": \"%s\"", "device", prop->dev);
    fprintf(STDDUMPIS, ", \"%s\": \"%s\"", "name", prop->name ? prop->name : "<null>");
    fprintf(STDDUMPIS, ", \"%s\": \"%s\"", "BLOBHandling", BL2TXT(prop->blob));
    fprintf(STDDUMPIS, "}");
}
void dumpClInfo(ClInfo* cp)
{
    int idx;
    int nmsgq  = cp->active && cp->msgq  ? nFQ(cp->msgq) : -1;
    int nprops = cp->active && cp->props ? cp->nprops    : -1;

    fprintf(STDDUMPIS, "{ \"%s\": %d", "active", cp->active);
    fprintf(STDDUMPIS, ", \"%s\": %d", "allprops", cp->allprops);
    fprintf(STDDUMPIS, ", \"%s\": \"%s\"", "BLOBHandling", BL2TXT(cp->blob));
    fprintf(STDDUMPIS, ", \"%s\": %d", "socketFD", cp->s);
    fprintf(STDDUMPIS, ", \"%s\": %u", "nsent", cp->nsent);
    fprintf(STDDUMPIS, ", \"%s\": \"%p\"", "gzfird", cp->gzfird);
    fprintf(STDDUMPIS, ", \"%s\": \"%p\"", "gzfiwr", cp->gzfiwr);
    fprintf(STDDUMPIS, ", \"%s\": %d", "gzwchk", cp->gzwchk);
    fprintf(STDDUMPIS, ", \"%s\": \"%p\"", "XML_parser", cp->lp);
    fprintf(STDDUMPIS, ", \"%s\": %d", "allprops", cp->allprops);
    fprintf(STDDUMPIS, ", \"%s\": \"%p\"", "props_pointer", cp->props);
    fprintf(STDDUMPIS, ", \"%s\": %d", "props_count", nprops);
    fprintf(STDDUMPIS, ", \"%s\": [\n", "props_list");
    for (idx=0; idx < nprops; ++idx)
    {
        DUMPIS1( idx ? ", " : "  ", dumpProperty(cp->props + idx));
    }
    if (-1 == nprops)
    {
      fprintf(STDDUMPIS, "%s: { \"noprops\": \"No Property list"
                         " generated here because Client object is not"
                         " active and/or Property* list pointer is"
                         " null\" }\n"
                       , dumpts
             );
    }
    fprintf(STDDUMPIS, "%s: ]\n", dumpts);

    fprintf(STDDUMPIS, "%s: , \"%s\": %d", dumpts, "msgq_count", nmsgq);
    fprintf(STDDUMPIS, ", \"%s\": \"%p\"", "msgq_pointer", cp->msgq);
    fprintf(STDDUMPIS, ", \"%s\": [\n", "msgq_list");
    for (idx=0; idx < nmsgq; ++idx)
    {
        DUMPIS1( idx ? ", " : "  ", dumpMsg(peekiFQ(cp->msgq, idx)));
    }
    if (-1 == nmsgq)
    {
      fprintf(STDDUMPIS, "%s: { \"nomsgq\": \"No Message list"
                         " generated here because Client object is not"
                         " active and/or FQ* message queue pointer is"
                         " null\" }\n"
                       , dumpts
             );
    }
    fprintf(STDDUMPIS, "%s: ]}", dumpts);
}
void dumpDvrInfo(DvrInfo* dp)
{
    int idx;
    int nmsgq   = dp->active && dp->msgq   ? nFQ(dp->msgq) : -1;
    int nsprops = dp->active && dp->sprops ? dp->nsprops   : -1;

    fprintf(STDDUMPIS, "{ \"%s\": \"%s\"", "name", dp->name);
    fprintf(STDDUMPIS, ", \"%s\": %d", "active", dp->active);
    fprintf(STDDUMPIS, ", \"%s\": %d", "isremot", dp->isremot);
    fprintf(STDDUMPIS, ", \"%s\": %d", "ndev", dp->ndev);
    fprintf(STDDUMPIS, ", \"%s\": %d", "nsent", dp->nsent);
    fprintf(STDDUMPIS, ", \"%s\": %d", "restartDelayus", dp->restartDelayus);
    fprintf(STDDUMPIS, ", \"%s\": %d", "restarts", dp->restarts);
    fprintf(STDDUMPIS, ", \"%s\": %d", "rfd", dp->rfd);
    fprintf(STDDUMPIS, ", \"%s\": %d", "wfd", dp->wfd);
    fprintf(STDDUMPIS, ", \"%s\": \"%p\"", "gzfird", dp->gzfird);
    fprintf(STDDUMPIS, ", \"%s\": \"%p\"", "gzfiwr", dp->gzfiwr);
    fprintf(STDDUMPIS, ", \"%s\": %d", "port", dp->port);
    fprintf(STDDUMPIS, ", \"%s\": \"%p\"", "XML_parser", dp->lp);
    fprintf(STDDUMPIS, ", \"%s\": \"%s\"", "host", dp->host);
    fprintf(STDDUMPIS, ", \"%s\": \"%s\"", "envDev", dp->envDev);
    fprintf(STDDUMPIS, ", \"%s\": \"%s\"", "evnConfig", dp->envConfig);
    fprintf(STDDUMPIS, ", \"%s\": \"%s\"", "envSkel", dp->envSkel);
    fprintf(STDDUMPIS, ", \"%s\": \"%s\"", "envPrefix", dp->envPrefix);

    fprintf(STDDUMPIS, ", \"%s\": \"%p\"", "sprops_pointer", dp->sprops);
    fprintf(STDDUMPIS, ", \"%s\": %d", "sprops_count", nsprops);
    fprintf(STDDUMPIS, ", \"%s\": [\n", "sprops_list");
    for (idx=0; idx < nsprops; ++idx)
    {
        DUMPIS1( idx ? ", " : "  ", dumpProperty(dp->sprops + idx));
    }
    if (-1 == nsprops)
    {
      fprintf(STDDUMPIS, "%s: { \"noprops\": \"No Property list"
                         " generated here because Driver object is not"
                         " active and/or Property* list pointer is"
                         " null\" }\n"
                       , dumpts
             );
    }
    fprintf(STDDUMPIS, "%s: ]\n", dumpts);

    fprintf(STDDUMPIS, "%s: , \"%s\": %d", dumpts, "msgq_count", nmsgq);
    fprintf(STDDUMPIS, ", \"%s\": \"%p\"", "msgq_pointer", dp->msgq);
    fprintf(STDDUMPIS, ", \"%s\": [\n", "msgq_list");
    for (idx=0; idx < nmsgq; ++idx)
    {
        DUMPIS1( idx ? ", " : "  ", dumpMsg(peekiFQ(dp->msgq, idx)));
    }
    if (-1 == nmsgq)
    {
      fprintf(STDDUMPIS, "%s: { \"nomsgq\": \"No Message list"
                         " generated here because Driver object is not"
                         " active and/or FQ* message queue pointer is"
                         " null\" }\n"
                       , dumpts
             );
    }

    fprintf(STDDUMPIS, "%s: ]}", dumpts);
}
void dumpis()
{
  dumpts = indi_tstamp(NULL);
  int idx;


  STDDUMPIS = stderr;

  DUMPIS1("", fprintf(STDDUMPIS, "[ { \"count\": %d , \"Clients\": [", nclinfo));
  for (idx=0; idx<nclinfo; ++idx)
  {
    DUMPIS1(idx ? ", " : "  ", dumpClInfo(clinfo+idx));
  }
  DUMPIS1("  ]}", "");

  DUMPIS1("", fprintf(STDDUMPIS, ", { \"count\": %d , \"Drivers\": [", ndvrinfo));
  for (idx=0; idx<ndvrinfo; ++idx)
  {
    DUMPIS1(idx ? "  , " : "    ", dumpDvrInfo(dvrinfo+idx));
  }
  DUMPIS1("  ]}", "");
  DUMPIS1("]", "");
}
#endif
