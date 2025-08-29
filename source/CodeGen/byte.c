#include "../../include/CodeGen/byte.h"
#include "../../include/CodeGen/chunk.h"
#include "../../include/CodeGen/memory.h"
#include "../../include/CodeGen/value.h"
#include "../../include/VM/vm.h"
#include <stdio.h>
#include <string.h>

/* magic costant che serve per identificare l'header del file bianrio */
static const uint8_t MAGIC[4] = { 'W', 'L', 'B', 1 };

/* tag per identificare le costanti */
typedef enum 
{   CT_NIL      = 0, 
    CT_BOOL     = 1, 
    CT_NUMBER   = 2, 
    CT_STRING   = 3, 
    CT_FUNCTION = 4 
}   ConstTag;

/* funzioni helper per scrivere/leggere da file */
static bool writeU8(FILE *file, uint8_t value)
{ 
    return (fwrite(&value, 1, 1, file) == 1);
}
static bool writeU32(FILE *file, uint32_t value)
{ 
    return (fwrite(&value, 4, 1, file) == 1);
}

static bool writeI32(FILE *file, int32_t value)
{ 
    return (fwrite(&value, 4, 1, file) == 1);
}

static bool writeD(FILE *file, double value)
{ 
    return (fwrite(&value, sizeof(value), 1, file) == 1);
}

static bool readU8(FILE *file, uint8_t *value)
{ 
    return (fread(value, 1, 1, file) == 1);
}

static bool readU32(FILE *file, uint32_t *value)
{ 
    return (fread(value, 4, 1, file) == 1);
}

static bool readI32(FILE *file, int32_t *value)
{ 
    return (fread(value, 4, 1, file) == 1);
}

static bool readD(FILE *file, double *value)
{ 
    return (fread(value, sizeof(*value), 1, file) == 1); 
}

/* funzione per scrivere una stringa sul file */
static bool writeString(FILE *file, ObjString* s)
{
    int32_t len = s ? (int32_t)s->lenght : -1;
    if (!writeI32(file, len)) return (false);
    if (len >= 0) return (fwrite(s->chars, 1, (size_t)len, file) == (size_t)len);
    return (true);
}

/* funzione per leggere una stringa dal file */
static ObjString *readString(FILE *file){
    int32_t len;

    if (!readI32(file, &len)) return (NULL);
    if (len < 0) return (NULL);

    char *buf = ALLOCATE(char, (size_t)len + 1);
    if (!buf)
    {
        printf("Errore nella lettura stringa da file bianrio!\n");
        return (NULL);
    }
    if (fread(buf, 1, (size_t)len, file) != (size_t)len) 
    {
        FREE_ARRAY(char, buf, len + 1); 
        return (NULL); 
    }
    buf[len] = '\0';
    return (takeString(buf, len));
}

/* forward declaration della funzione che scrive una funzione su file */
static bool writeFunctionBody(FILE *file, ObjFunction *fn);

/* funzione che scrive le costanti sul file (ca = costants array) */
static bool writeConstants(FILE *file, ValueArray *ca){
    if (!writeU32(file, (uint32_t)ca->count)) return (false);
    
    for (int i = 0; i < ca->count; i++)
    {
        Value v = ca->values[i];
        if (IS_NIL(v))
        {
            if (!writeU8(file, CT_NIL)) return (false);
        }
        else if (IS_BOOL(v))
        {
            if (!writeU8(file, CT_BOOL)) return (false);
            if (!writeU8(file, AS_BOOL(v) ? 1 : 0)) return (false);
        } 
        else if (IS_NUMBER(v))
        {
            if (!writeU8(file, CT_NUMBER)) return (false);
            if (!writeD(file, AS_NUMBER(v))) return (false);
        } 
        else if (IS_OBJ(v)) 
        {
            switch (OBJ_TYPE(v))
            {
                case OBJ_STRING:
                {
                    if (!writeU8(file, CT_STRING)) return (false);
                    if (!writeString(file, AS_STRING(v))) return (false);
                    break;
                }
                case OBJ_FUNCTION:
                {
                    if (!writeU8(file, CT_FUNCTION)) return (false);
                    if (!writeFunctionBody(file, AS_FUNCTION(v))) return (false);
                    break;
                }
                default:
                    return (false);
            }
        } 
        else
        { 
            return (false);
        }
    }
    return (true);
}
/* forward declaration della funzione che legge le costati dal file */
static bool readConstants(FILE *file, ValueArray *ca);

/* funzione che legge il corpo di una funzione dal file */
static ObjFunction *readFunctionBody(FILE *file)
{
    ObjFunction *fn = newFunction();
    uint8_t ar; 
    
    if (!readU8(file, &ar)) return (NULL);
    fn->arity = ar;
    fn->name = readString(file);

    uint32_t codeCount; 
    if (!readU32(file, &codeCount)) return (NULL);
    fn->chunk.count = (int)codeCount;
    fn->chunk.capacity = (int)codeCount;
    if (codeCount)
    {
        fn->chunk.code = ALLOCATE(uint8_t, codeCount);
        if (fread(fn->chunk.code, 1, codeCount, file) != codeCount) return (NULL);
    }

    uint32_t linesCount; 
    if (!readU32(file, &linesCount)) return (NULL);
    if (linesCount)
    {
        fn->chunk.lines = ALLOCATE(int, linesCount);
        if (fread(fn->chunk.lines, sizeof(int), linesCount, file) != linesCount) return (NULL);
    }

    if (!readConstants(file, &fn->chunk.constants)) return (NULL);
    return (fn);
}

/* funzione che legge le costati dal file */
static bool readConstants(FILE *file, ValueArray *ca){
    uint32_t count; 
    if (!readU32(file, &count)) return (false);
    
    for (uint32_t i = 0; i < count; i++)
    {
        uint8_t tag; 
        if (!readU8(file, &tag)) return (false);
        switch (tag)
        {
            case CT_NIL: writeValueArray(ca, NIL_VAL); break;
            case CT_BOOL: 
            { 
                uint8_t b; 
                if (!readU8(file, &b)) return (false); 
                writeValueArray(ca, BOOL_VAL(b!=0)); 
                break; 
            }
            case CT_NUMBER: 
            { 
                double d; 
                if (!readD(file, &d)) return (false); 
                writeValueArray(ca, NUMBER_VAL(d)); 
                break; 
            }
            case CT_STRING: 
            {
                ObjString *s = readString(file); 
                if (!s) return (false);
                writeValueArray(ca, OBJ_VAL(s)); 
                break;
            }
            case CT_FUNCTION: 
            {
                ObjFunction *sub = readFunctionBody(file); 
                if (!sub) return (false);
                writeValueArray(ca, OBJ_VAL(sub)); 
                break;
            }
            default: 
                return (false);
        }
    }
    return (true);
}

/* funzione che scrive su file il body di una funzione */
static bool writeFunctionBody(FILE *file, ObjFunction *fn){
    if (!writeU8(file, (uint8_t)fn->arity)) return (false);
    if (!writeString(file, fn->name)) return (false);
    if (!writeU32(file, (uint32_t)fn->chunk.count)) return (false);
    if (fn->chunk.count && fwrite(fn->chunk.code, 1, (size_t)fn->chunk.count, file) != (size_t)fn->chunk.count) return (false);
    if (!writeU32(file, (uint32_t)fn->chunk.count)) return (false); 
    if (fn->chunk.count && fwrite(fn->chunk.lines, sizeof(int), (size_t)fn->chunk.count, file) != (size_t)fn->chunk.count) return (false);
    return (writeConstants(file, &fn->chunk.constants));
}

/* funzione che scrive la funzione sul file */
bool writeFunctionToFile(ObjFunction *fn, const char *filename){
    FILE *f = fopen(filename, "wb");
    if (!f) return (false);
    bool ok = (fwrite(MAGIC, 1, 4, f) == 4) && writeFunctionBody(f, fn);
    fclose(f);
    return (ok);
}
/* funzione che legge funzione da file */
ObjFunction* readFunctionFromFile(const char *filename){
    FILE *f = fopen(filename, "rb");
    if (!f) return (NULL);

    uint8_t head[4];
    if (fread(head, 1, 4, f) != 4 || memcmp(head, MAGIC, 4) != 0 )
    {
        fclose(f); 
        return (NULL); 
    }

    ObjFunction *fn = readFunctionBody(f);
    fclose(f);
    return (fn);
}