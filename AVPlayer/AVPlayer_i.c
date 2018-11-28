

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Nov 28 15:00:11 2018
 */
/* Compiler settings for AVPlayer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_AVPlayerLib,0xACE87B96,0xB12B,0x467F,0x87,0x03,0x44,0xCD,0xEF,0xBA,0x54,0xDF);


MIDL_DEFINE_GUID(IID, DIID__DAVPlayer,0x7FA42197,0x3260,0x4BBB,0x99,0xF9,0x28,0x36,0xE5,0x3B,0xB0,0x3B);


MIDL_DEFINE_GUID(IID, DIID__DAVPlayerEvents,0x789C77B5,0x9570,0x491B,0x9A,0x70,0x6B,0xC7,0x69,0x71,0xB5,0x30);


MIDL_DEFINE_GUID(CLSID, CLSID_AVPlayer,0x7CB0D592,0x1870,0x4608,0x81,0x3B,0x8D,0xBB,0xCD,0x3D,0xAB,0x57);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



