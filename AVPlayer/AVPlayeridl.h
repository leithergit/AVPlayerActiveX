

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Nov 23 11:44:41 2018
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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __AVPlayeridl_h__
#define __AVPlayeridl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DAVPlayer_FWD_DEFINED__
#define ___DAVPlayer_FWD_DEFINED__
typedef interface _DAVPlayer _DAVPlayer;
#endif 	/* ___DAVPlayer_FWD_DEFINED__ */


#ifndef ___DAVPlayerEvents_FWD_DEFINED__
#define ___DAVPlayerEvents_FWD_DEFINED__
typedef interface _DAVPlayerEvents _DAVPlayerEvents;
#endif 	/* ___DAVPlayerEvents_FWD_DEFINED__ */


#ifndef __AVPlayer_FWD_DEFINED__
#define __AVPlayer_FWD_DEFINED__

#ifdef __cplusplus
typedef class AVPlayer AVPlayer;
#else
typedef struct AVPlayer AVPlayer;
#endif /* __cplusplus */

#endif 	/* __AVPlayer_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __AVPlayerLib_LIBRARY_DEFINED__
#define __AVPlayerLib_LIBRARY_DEFINED__

/* library AVPlayerLib */
/* [control][helpstring][helpfile][version][uuid] */ 

typedef /* [v1_enum][helpstring] */ 
enum PtzCode
    {	Ptz_Center	= 0,
	Ptz_Areazoom	= ( Ptz_Center + 1 ) ,
	Ptz_ImageWidth	= ( Ptz_Areazoom + 1 ) ,
	Ptz_ImageHeight	= ( Ptz_ImageWidth + 1 ) ,
	Ptz_Move	= ( Ptz_ImageHeight + 1 ) ,
	Ptz_Pan	= ( Ptz_Move + 1 ) ,
	Ptz_Tilt	= ( Ptz_Pan + 1 ) ,
	Ptz_Zoom	= ( Ptz_Tilt + 1 ) ,
	Ptz_Focus	= ( Ptz_Zoom + 1 ) ,
	Ptz_Iris	= ( Ptz_Focus + 1 ) ,
	Ptz_Brightness	= ( Ptz_Iris + 1 ) ,
	Ptz_AutoFocus	= ( Ptz_Brightness + 1 ) ,
	Ptz_AutoIris	= ( Ptz_AutoFocus + 1 ) ,
	Ptz_ContinuousPanTiltMove	= ( Ptz_AutoIris + 1 ) ,
	Ptz_ContinuousFocusMove	= ( Ptz_ContinuousPanTiltMove + 1 ) ,
	Ptz_ContinuousZoomMove	= ( Ptz_ContinuousFocusMove + 1 ) ,
	Ptz_ContinuousIrisMove	= ( Ptz_ContinuousZoomMove + 1 ) ,
	Ptz_ContinuousBrightnessMove	= ( Ptz_ContinuousIrisMove + 1 ) ,
	Ptz_GogoServerPresetNo	= ( Ptz_ContinuousBrightnessMove + 1 ) ,
	Ptz_Speed	= ( Ptz_GogoServerPresetNo + 1 ) ,
	Ptz_IrcutFilter	= ( Ptz_Speed + 1 ) ,
	Ptz_BackLight	= ( Ptz_IrcutFilter + 1 ) 
    } 	PtzCode;

typedef /* [v1_enum][helpstring] */ 
enum AVStatus
    {	AvError_Succeed	= 0,
	AvError_base	= -1024,
	AvError_InvalidParameters	= ( AvError_base - 1 ) ,
	AvError_Invalid_ServerAddress	= ( AvError_base - 2 ) ,
	AvError_Invliad_ServerPort	= ( AvError_base - 3 ) ,
	AvError_Invalid_AccountOrPassword	= ( AvError_base - 4 ) ,
	AvError_ConnectServerFailed	= ( AvError_base - 5 ) ,
	AvError_LoginFailed	= ( AvError_base - 6 ) ,
	AvError_NotLogintoServer	= ( AvError_base - 7 ) ,
	AvError_InvalidWindow	= ( AvError_base - 8 ) ,
	AvError_DeviceNotExist	= ( AvError_base - 9 ) ,
	AvError_ConnectDeviceFailed	= ( AvError_base - 10 ) ,
	AvError_DeviceInPlaying	= ( AvError_base - 11 ) ,
	AvError_DBException	= ( AvError_base - 12 ) ,
	AvError_DeviceNotInPlaying	= ( AvError_base - 13 ) ,
	AVError_BufferOverflow	= ( AvError_base - 14 ) ,
	AvError_WindowNotPlaying	= ( AvError_base - 15 ) ,
	AvError_InvlaidPtzCommand	= ( AvError_base - 16 ) ,
	AvError_Invalid_PtzValue	= ( AvError_base - 17 ) ,
	AvError_ExternalError	= ( AvError_base - 253 ) ,
	AvError_InsufficentMemory	= ( AvError_base - 254 ) ,
	AvError_UnknownException	= ( AvError_base - 255 ) 
    } 	AVStatus;

typedef /* [v1_enum][helpstring] */ 
enum Position
    {	Pos_Left	= 0,
	Pos_Top	= ( Pos_Left + 1 ) ,
	Pos_Right	= ( Pos_Top + 1 ) ,
	Pos_Bottom	= ( Pos_Right + 1 ) 
    } 	Position;

typedef /* [v1_enum][helpstring] */ 
enum Ptz_MoveOption
    {	Mov_Home	= 0,
	Mov_Up	= ( Mov_Home + 1 ) ,
	Mov_Down	= ( Mov_Up + 1 ) ,
	Mov_Left	= ( Mov_Down + 1 ) ,
	Mov_Right	= ( Mov_Left + 1 ) ,
	Mov_Upleft	= ( Mov_Right + 1 ) ,
	Mov_UPright	= ( Mov_Upleft + 1 ) ,
	Mov_Downleft	= ( Mov_UPright + 1 ) ,
	Mov_Downright	= ( Mov_Downleft + 1 ) ,
	Mov_Stop	= ( Mov_Downright + 1 ) 
    } 	Ptz_MoveOption;

typedef /* [v1_enum][helpstring] */ 
enum PtzOption
    {	Opt_Abstract	= 1,
	Opt_Relative	= 0
    } 	PtzOption;


EXTERN_C const IID LIBID_AVPlayerLib;

#ifndef ___DAVPlayer_DISPINTERFACE_DEFINED__
#define ___DAVPlayer_DISPINTERFACE_DEFINED__

/* dispinterface _DAVPlayer */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DAVPlayer;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7FA42197-3260-4BBB-99F9-2836E53BB03B")
    _DAVPlayer : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DAVPlayerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DAVPlayer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DAVPlayer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DAVPlayer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DAVPlayer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DAVPlayer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DAVPlayer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DAVPlayer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DAVPlayerVtbl;

    interface _DAVPlayer
    {
        CONST_VTBL struct _DAVPlayerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DAVPlayer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DAVPlayer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DAVPlayer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DAVPlayer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DAVPlayer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DAVPlayer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DAVPlayer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DAVPlayer_DISPINTERFACE_DEFINED__ */


#ifndef ___DAVPlayerEvents_DISPINTERFACE_DEFINED__
#define ___DAVPlayerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DAVPlayerEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DAVPlayerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("789C77B5-9570-491B-9A70-6BC76971B530")
    _DAVPlayerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DAVPlayerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DAVPlayerEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DAVPlayerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DAVPlayerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DAVPlayerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DAVPlayerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DAVPlayerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DAVPlayerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DAVPlayerEventsVtbl;

    interface _DAVPlayerEvents
    {
        CONST_VTBL struct _DAVPlayerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DAVPlayerEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DAVPlayerEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DAVPlayerEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DAVPlayerEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DAVPlayerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DAVPlayerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DAVPlayerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DAVPlayerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_AVPlayer;

#ifdef __cplusplus

class DECLSPEC_UUID("7CB0D592-1870-4608-813B-8DBBCD3DAB57")
AVPlayer;
#endif
#endif /* __AVPlayerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


