#include "../include/DumpTools.h"

int WriteIndent(FILE* pF, int count)
{
    int cnt = 0;
    for(int i=0;i<count;i++) {
        cnt += fprintf(pF, "\t");
    }
    return cnt;
}

int WriteXmlHeader(FILE* pF, int indent = 0)
{
    int cnt = 0;

    cnt += WriteIndent(pF, indent);
    cnt += fprintf(pF, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n");
    return cnt;
}

int WriteXmlTagStart(FILE* pF, char* tag, int indent = 0, int attibute_count = 0, ...)
{
    int cnt = 0;
    va_list argptr;

    cnt += WriteIndent(pF, indent);
    cnt += fprintf(pF, "<%s", tag);
    if(attibute_count > 0) {
        va_start(argptr, attibute_count);
        for(int i=0;i<attibute_count;i++) {
            char* attribute = va_arg(argptr, char*);
            char* value = va_arg(argptr, char*);
            cnt += fprintf(pF, " %s=\"%s\"", attribute, value);
        }
        va_end(argptr);
    }
    cnt += fprintf(pF, ">\n");
    return cnt;
}

int WriteXmlTagEnd(FILE* pF, char* tag, int indent = 0)
{
    int cnt = 0;

    cnt += WriteIndent(pF, indent);
    cnt += fprintf(pF, "</%s>\n", tag);
    return cnt;
}

int WriteXmlTagClosed(FILE* pF, char* tag, int indent = 0, int attibute_count = 0, ...)
{
    int cnt = 0;
    va_list argptr;

    cnt += WriteIndent(pF, indent);
    cnt += fprintf(pF, "<%s", tag);
    if(attibute_count > 0) {
        va_start(argptr, attibute_count);
        for(int i=0;i<attibute_count;i++) {
            char* attribute = va_arg(argptr, char*);
            char* value = va_arg(argptr, char*);
            cnt += fprintf(pF, " %s=\"%s\"", attribute, value);
        }
        va_end(argptr);
    }
    cnt += fprintf(pF, "/>\n");
    return cnt;
}

int WriteXmlComment(FILE* pF, char* comment, int indent = 0)
{
    int cnt = 0;

    cnt += WriteIndent(pF, indent);
    cnt += fprintf(pF, "<!-- %s -->\n", comment);
    return cnt;
}

DWORD* GetArchivePointer(CMwNod* nod)
{
    DWORD* nodVtable;
    nodVtable = (DWORD*)nod;
	nodVtable = (DWORD*)nodVtable[0];
	return (DWORD*)nodVtable[15];
}


int DumpTools::DumpChunkInfos(char* path)
{
    // WARNING! THIS FUNCTION ***WILL*** CRASH THE GAME AFTER IT'S DONE!
    // The reason is some classes are "global" and will cause issues when creating new instances.
    // Like CTrackMania for example.
    // The function will work correctly, but please save any work before using it.

    char* tag_main = "game";
    char* tag_engine = "engine";
    char* tag_class = "class";
    char* tag_chunk = "chunk";

    DWORD* base_archive;

    printf("Chunk info dump begin\n");
    FILE* pF = fopen(path, "w");
    if(!pF) {
        return 1;
    }
    CMwEngineManager* engine_man = GbxTools::GetEngineManager();
    CMwNod* base_nod = GbxTools::GetClassInfoByMwClassId(0x01001000)->m_Constructor();
	base_archive = GetArchivePointer(base_nod);
	base_nod->Delete(1);

    WriteXmlHeader(pF);
    WriteXmlTagStart(pF, tag_main, 0, 1, "name", "TrackMania 1.0");
    // For Each Engine
    for (int i = 0; i < engine_man->m_Engines.numElems; i++) {
        CMwEngineInfo* engineinfo = engine_man->m_Engines.pElems[i];
        if (engineinfo != NULL) {
            printf("Engine \"%s\" (%08X)\n", engineinfo->m_EngineName, engineinfo->m_EngineId);
            char* engine_id = (char*)malloc(16);
            sprintf_s(engine_id, 16, "%02X", engineinfo->m_EngineId >> 24);
            WriteXmlTagStart(pF, tag_engine, 1, 2,
                "id", engine_id,
                "name", engineinfo->m_EngineName);
            free(engine_id);
            // For Each Class
            for (int j = 0; j < engineinfo->m_Classes.numElems; j++) {
                CMwClassInfo* classinfo = engineinfo->m_Classes.pElems[j];
                if (classinfo != NULL) {
                    printf("\tClass \"%s\" (%08X)... ", classinfo->m_ClassName, classinfo->m_ClassId);
                    char* class_id = (char*)malloc(16);
                    sprintf_s(class_id, 16, "%08X", classinfo->m_ClassId);

                    if (classinfo->m_Constructor != NULL) {
                        printf("calling constructor... ");
                        CMwNod* nod = classinfo->m_Constructor();
                        DWORD* current_archive = GetArchivePointer(nod);

                        char* str_chunk_count = (char*)malloc(16);
                        sprintf_s(str_chunk_count, 16, "%i", nod->GetChunkCount());

                        CMwClassInfo* parent_class = classinfo->m_ParentClass;
                        if(parent_class) {
                            if(base_archive == current_archive) {
                                WriteXmlTagStart(pF, tag_class, 2, 4,
                                    "id", class_id,
                                    "name", classinfo->m_ClassName,
                                    "parent", parent_class->m_ClassName,
                                    "chunks", str_chunk_count
                                );
                            } else {
                                WriteXmlTagStart(pF, tag_class, 2, 4,
                                    "id", class_id,
                                    "name", classinfo->m_ClassName,
                                    "parent", parent_class->m_ClassName,
                                    "custom_archive", "1"
                                );
                            }
                        } else {
                            if(base_archive == current_archive) {
                                WriteXmlTagStart(pF, tag_class, 2, 3,
                                    "id", class_id,
                                    "name", classinfo->m_ClassName,
                                    "chunks", str_chunk_count
                                );
                            } else {
                                WriteXmlTagStart(pF, tag_class, 2, 3,
                                    "id", class_id,
                                    "name", classinfo->m_ClassName,
                                    "custom_archive", "1"
                                );

                            }
                        }
                        free(str_chunk_count);

                        if(nod->GetChunkCount() == 1)
                            continue; // le bugg

                        int first_chunk_id = nod->GetUidChunkFromIndex(1);
                        CMwClassInfo* current_class_info = GbxTools::GetClassInfoByMwClassId(first_chunk_id & 0xFFFFF000);
                        WriteXmlComment(pF, current_class_info->m_ClassName, 3);

                        for(int k=0;k<nod->GetChunkCount();k++) {
                            int chunk_id = nod->GetUidChunkFromIndex(k+1);
                            int chunk_info = nod->GetChunkInfo(chunk_id);

                            CMwClassInfo* next_class_info = GbxTools::GetClassInfoByMwClassId(chunk_id & 0xFFFFF000);
                            if(next_class_info && current_class_info->m_ClassId != next_class_info->m_ClassId) {
                                current_class_info = next_class_info;
                                WriteXmlComment(pF, current_class_info->m_ClassName, 3);
                            }

                            char* str_chunk_id = (char*)malloc(16);
                            sprintf_s(str_chunk_id, 16, "%08X", chunk_id);
                            char* str_chunk_info = (char*)malloc(16);
                            sprintf_s(str_chunk_info, 16, "%08X", chunk_info);

                            WriteXmlTagClosed(pF, tag_chunk, 3, 2,
                                "id", str_chunk_count,
                                "info", str_chunk_info
                            );

                            free(str_chunk_id);
                            free(str_chunk_info);
                        }

                        //test->Delete(0); // i should delete the nod, but it causes crashes when doing so, monkaS

                        WriteXmlTagEnd(pF, tag_class, 2);
                    } else {
                        printf("SKIP! (no constructor)");
                        CMwClassInfo* parent_class = classinfo->m_ParentClass;
                        if(parent_class) {
                            WriteXmlTagStart(pF, tag_class, 2, 4,
                                "id", class_id,
                                "name", classinfo->m_ClassName,
                                "parent", parent_class->m_ClassName,
                                "skipped", "1"
                            );
                        } else {
                            WriteXmlTagStart(pF, tag_class, 2, 3,
                                "id", class_id,
                                "name", classinfo->m_ClassName,
                                "skipped", "1"
                            );
                        }
                    }
                    free(class_id);
                    printf("\n");
                }
            }
            WriteXmlTagEnd(pF, tag_engine, 1);
        }
    }
    WriteXmlTagEnd(pF, tag_main);

    fclose(pF);
    return 0;
}
