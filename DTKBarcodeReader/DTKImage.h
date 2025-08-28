

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Fri Sep 11 17:13:25 2015
 */
/* Compiler settings for .\DTKImage.idl:
    Oicf, W1, Zp8, env=Win64 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __DTKImage_h__
#define __DTKImage_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAnnotationCollection_FWD_DEFINED__
#define __IAnnotationCollection_FWD_DEFINED__
typedef interface IAnnotationCollection IAnnotationCollection;
#endif 	/* __IAnnotationCollection_FWD_DEFINED__ */


#ifndef __IImageObject_FWD_DEFINED__
#define __IImageObject_FWD_DEFINED__
typedef interface IImageObject IImageObject;
#endif 	/* __IImageObject_FWD_DEFINED__ */


#ifndef __IImageObjectCollection_FWD_DEFINED__
#define __IImageObjectCollection_FWD_DEFINED__
typedef interface IImageObjectCollection IImageObjectCollection;
#endif 	/* __IImageObjectCollection_FWD_DEFINED__ */


#ifndef __IImageViewer_FWD_DEFINED__
#define __IImageViewer_FWD_DEFINED__
typedef interface IImageViewer IImageViewer;
#endif 	/* __IImageViewer_FWD_DEFINED__ */


#ifndef __IAnnotationLine_FWD_DEFINED__
#define __IAnnotationLine_FWD_DEFINED__
typedef interface IAnnotationLine IAnnotationLine;
#endif 	/* __IAnnotationLine_FWD_DEFINED__ */


#ifndef __IAnnotationRectangle_FWD_DEFINED__
#define __IAnnotationRectangle_FWD_DEFINED__
typedef interface IAnnotationRectangle IAnnotationRectangle;
#endif 	/* __IAnnotationRectangle_FWD_DEFINED__ */


#ifndef __ITIFFUtils_FWD_DEFINED__
#define __ITIFFUtils_FWD_DEFINED__
typedef interface ITIFFUtils ITIFFUtils;
#endif 	/* __ITIFFUtils_FWD_DEFINED__ */


#ifndef __ITwain_FWD_DEFINED__
#define __ITwain_FWD_DEFINED__
typedef interface ITwain ITwain;
#endif 	/* __ITwain_FWD_DEFINED__ */


#ifndef __IPDFUtils_FWD_DEFINED__
#define __IPDFUtils_FWD_DEFINED__
typedef interface IPDFUtils IPDFUtils;
#endif 	/* __IPDFUtils_FWD_DEFINED__ */


#ifndef __ImageObject_FWD_DEFINED__
#define __ImageObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class ImageObject ImageObject;
#else
typedef struct ImageObject ImageObject;
#endif /* __cplusplus */

#endif 	/* __ImageObject_FWD_DEFINED__ */


#ifndef __ImageObjectCollection_FWD_DEFINED__
#define __ImageObjectCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class ImageObjectCollection ImageObjectCollection;
#else
typedef struct ImageObjectCollection ImageObjectCollection;
#endif /* __cplusplus */

#endif 	/* __ImageObjectCollection_FWD_DEFINED__ */


#ifndef ___IImageViewerEvents_FWD_DEFINED__
#define ___IImageViewerEvents_FWD_DEFINED__
typedef interface _IImageViewerEvents _IImageViewerEvents;
#endif 	/* ___IImageViewerEvents_FWD_DEFINED__ */


#ifndef __ImageViewer_FWD_DEFINED__
#define __ImageViewer_FWD_DEFINED__

#ifdef __cplusplus
typedef class ImageViewer ImageViewer;
#else
typedef struct ImageViewer ImageViewer;
#endif /* __cplusplus */

#endif 	/* __ImageViewer_FWD_DEFINED__ */


#ifndef __AnnotationLine_FWD_DEFINED__
#define __AnnotationLine_FWD_DEFINED__

#ifdef __cplusplus
typedef class AnnotationLine AnnotationLine;
#else
typedef struct AnnotationLine AnnotationLine;
#endif /* __cplusplus */

#endif 	/* __AnnotationLine_FWD_DEFINED__ */


#ifndef __AnnotationCollection_FWD_DEFINED__
#define __AnnotationCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class AnnotationCollection AnnotationCollection;
#else
typedef struct AnnotationCollection AnnotationCollection;
#endif /* __cplusplus */

#endif 	/* __AnnotationCollection_FWD_DEFINED__ */


#ifndef __AnnotationRectangle_FWD_DEFINED__
#define __AnnotationRectangle_FWD_DEFINED__

#ifdef __cplusplus
typedef class AnnotationRectangle AnnotationRectangle;
#else
typedef struct AnnotationRectangle AnnotationRectangle;
#endif /* __cplusplus */

#endif 	/* __AnnotationRectangle_FWD_DEFINED__ */


#ifndef ___ITIFFUtilsEvents_FWD_DEFINED__
#define ___ITIFFUtilsEvents_FWD_DEFINED__
typedef interface _ITIFFUtilsEvents _ITIFFUtilsEvents;
#endif 	/* ___ITIFFUtilsEvents_FWD_DEFINED__ */


#ifndef __TIFFUtils_FWD_DEFINED__
#define __TIFFUtils_FWD_DEFINED__

#ifdef __cplusplus
typedef class TIFFUtils TIFFUtils;
#else
typedef struct TIFFUtils TIFFUtils;
#endif /* __cplusplus */

#endif 	/* __TIFFUtils_FWD_DEFINED__ */


#ifndef ___IPDFUtilsEvents_FWD_DEFINED__
#define ___IPDFUtilsEvents_FWD_DEFINED__
typedef interface _IPDFUtilsEvents _IPDFUtilsEvents;
#endif 	/* ___IPDFUtilsEvents_FWD_DEFINED__ */


#ifndef __PDFUtils_FWD_DEFINED__
#define __PDFUtils_FWD_DEFINED__

#ifdef __cplusplus
typedef class PDFUtils PDFUtils;
#else
typedef struct PDFUtils PDFUtils;
#endif /* __cplusplus */

#endif 	/* __PDFUtils_FWD_DEFINED__ */


#ifndef ___ITwainEvents_FWD_DEFINED__
#define ___ITwainEvents_FWD_DEFINED__
typedef interface _ITwainEvents _ITwainEvents;
#endif 	/* ___ITwainEvents_FWD_DEFINED__ */


#ifndef __Twain_FWD_DEFINED__
#define __Twain_FWD_DEFINED__

#ifdef __cplusplus
typedef class Twain Twain;
#else
typedef struct Twain Twain;
#endif /* __cplusplus */

#endif 	/* __Twain_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DTKImage_0000_0000 */
/* [local] */ 

typedef /* [public][v1_enum] */ 
enum ImageFormatEnum
    {	IF_Unknown	= 0,
	IF_BMP	= 1,
	IF_PNG	= 2,
	IF_JPEG	= 3,
	IF_TIFF	= 4
    } 	ImageFormatEnum;

typedef /* [public][v1_enum] */ 
enum AutoSizeModeEnum
    {	ASM_None	= 0,
	ASM_Stretch	= 1,
	ASM_FitToWidth	= 2,
	ASM_FitToHeight	= 3,
	ASM_BestFit	= 4
    } 	AutoSizeModeEnum;

typedef /* [public][v1_enum] */ 
enum BinarizationMethodEnum
    {	BM_Simple	= 0,
	BM_ErrorDiffusion	= 1
    } 	BinarizationMethodEnum;

typedef /* [public][v1_enum] */ 
enum InterpolationMethodEnum
    {	IM_NEAREST_NEIGHBOUR	= 1,
	IM_BILINEAR	= 2,
	IM_BSPLINE	= 3,
	IM_BICUBIC	= 4,
	IM_BICUBIC2	= 5,
	IM_LANCZOS	= 6,
	IM_BOX	= 7,
	IM_HERMITE	= 8,
	IM_HAMMING	= 9,
	IM_SINC	= 10,
	IM_BLACKMAN	= 11,
	IM_BESSEL	= 12,
	IM_GAUSSIAN	= 13,
	IM_QUADRATIC	= 14,
	IM_MITCHELL	= 15,
	IM_CATROM	= 16
    } 	InterpolationMethodEnum;

typedef /* [public][v1_enum] */ 
enum OverflowMethodEnum
    {	OM_COLOR	= 1,
	OM_BACKGROUND	= 2,
	OM_TRANSPARENT	= 3,
	OM_WRAP	= 4,
	OM_REPEAT	= 5,
	OM_MIRROR	= 6
    } 	OverflowMethodEnum;

typedef /* [public][v1_enum] */ 
enum TIFFCompressionEnum
    {	TIFFC_Unknown	= 0,
	TIFFC_None	= 1,
	TIFFC_CCITTRLE	= 2,
	TIFFC_CCITTFAX3	= 3,
	TIFFC_CCITTFAX4	= 4,
	TIFFC_LZW	= 5,
	TIFFC_OJPEG	= 6,
	TIFFC_JPEG	= 7,
	TIFFC_PACKBITS	= 32773
    } 	TIFFCompressionEnum;




extern RPC_IF_HANDLE __MIDL_itf_DTKImage_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DTKImage_0000_0000_v0_0_s_ifspec;

#ifndef __IAnnotationCollection_INTERFACE_DEFINED__
#define __IAnnotationCollection_INTERFACE_DEFINED__

/* interface IAnnotationCollection */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IAnnotationCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C6D0552B-EC2F-4268-B870-F3A411DC856C")
    IAnnotationCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ LONG ind,
            /* [retval][out] */ IUnknown **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddLine( 
            LONG x1,
            LONG y1,
            LONG x2,
            LONG y2,
            OLE_COLOR color,
            LONG frame) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddRectangle( 
            LONG left,
            LONG top,
            LONG right,
            LONG bottom,
            OLE_COLOR color,
            LONG frame) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAnnotationCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAnnotationCollection * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAnnotationCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAnnotationCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAnnotationCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAnnotationCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAnnotationCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAnnotationCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IAnnotationCollection * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IAnnotationCollection * This,
            /* [in] */ LONG ind,
            /* [retval][out] */ IUnknown **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddLine )( 
            IAnnotationCollection * This,
            LONG x1,
            LONG y1,
            LONG x2,
            LONG y2,
            OLE_COLOR color,
            LONG frame);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddRectangle )( 
            IAnnotationCollection * This,
            LONG left,
            LONG top,
            LONG right,
            LONG bottom,
            OLE_COLOR color,
            LONG frame);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IAnnotationCollection * This);
        
        END_INTERFACE
    } IAnnotationCollectionVtbl;

    interface IAnnotationCollection
    {
        CONST_VTBL struct IAnnotationCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAnnotationCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAnnotationCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAnnotationCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAnnotationCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAnnotationCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAnnotationCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAnnotationCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IAnnotationCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IAnnotationCollection_get_Item(This,ind,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,ind,pVal) ) 

#define IAnnotationCollection_AddLine(This,x1,y1,x2,y2,color,frame)	\
    ( (This)->lpVtbl -> AddLine(This,x1,y1,x2,y2,color,frame) ) 

#define IAnnotationCollection_AddRectangle(This,left,top,right,bottom,color,frame)	\
    ( (This)->lpVtbl -> AddRectangle(This,left,top,right,bottom,color,frame) ) 

#define IAnnotationCollection_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAnnotationCollection_INTERFACE_DEFINED__ */


#ifndef __IImageObject_INTERFACE_DEFINED__
#define __IImageObject_INTERFACE_DEFINED__

/* interface IImageObject */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IImageObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("305D0862-D37B-4F2B-BFF7-282AD497721A")
    IImageObject : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_xDPI( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_yDPI( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFromFile( 
            BSTR fileName,
            LONG frame) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFromBitmap( 
            LONG hBmp,
            LONG hPalette) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFromDIB( 
            LONG hDIB) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFromMemFile( 
            SAFEARRAY * *fileData,
            LONG dataLen,
            LONG frame) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Bpp( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveToFileTIFF( 
            BSTR fileName,
            TIFFCompressionEnum compression) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveToFileJPEG( 
            BSTR fileName,
            LONG quality) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveToFileBMP( 
            BSTR fileName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveToFilePNG( 
            BSTR fileName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ImageViewer( 
            /* [retval][out] */ IImageViewer **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ConvertToGrayScale( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Flip( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Mirror( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Negative( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RotateRight( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RotateLeft( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Rotate( 
            FLOAT angle,
            InterpolationMethodEnum inMethod,
            OverflowMethodEnum ofMethod,
            VARIANT_BOOL bKeepOriginalSize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Binarize( 
            BinarizationMethodEnum method) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSkewAngle( 
            FLOAT *angle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Despeckle( 
            LONG ksize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Dilate( 
            LONG ksize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Erode( 
            LONG ksize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsBlank( 
            LONG threshold,
            VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangeBrightnessContrast( 
            LONG brightness,
            LONG contrast) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsGrayScale( 
            VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumColors( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HBITMAP( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FrameCount( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Frame( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFrame( 
            LONG frame) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Size( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Handle( 
            /* [retval][out] */ OLE_HANDLE *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLastErrorText( 
            BSTR *errorText) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Save( 
            BSTR fileName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Annotations( 
            /* [retval][out] */ IAnnotationCollection **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFromBuffer( 
            LONG pBuffer,
            LONG width,
            LONG height,
            LONG stride,
            LONG bpp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TooLight( 
            LONG threshold,
            VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TooDark( 
            LONG threshold,
            VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FileName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsBlankRect( 
            LONG threshold,
            LONG left,
            LONG top,
            LONG right,
            LONG bottom,
            VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Crop( 
            LONG left,
            LONG top,
            LONG right,
            LONG bottom,
            IImageObject **image) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFromFile2( 
            BSTR fileName,
            LONG frame,
            BSTR password) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFromMemFile2( 
            SAFEARRAY * *fileData,
            LONG dataLen,
            LONG frame,
            BSTR password) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IImageObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IImageObject * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IImageObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IImageObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IImageObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IImageObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IImageObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IImageObject * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IImageObject * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IImageObject * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_xDPI )( 
            IImageObject * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_yDPI )( 
            IImageObject * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFromFile )( 
            IImageObject * This,
            BSTR fileName,
            LONG frame);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFromBitmap )( 
            IImageObject * This,
            LONG hBmp,
            LONG hPalette);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFromDIB )( 
            IImageObject * This,
            LONG hDIB);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFromMemFile )( 
            IImageObject * This,
            SAFEARRAY * *fileData,
            LONG dataLen,
            LONG frame);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Bpp )( 
            IImageObject * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveToFileTIFF )( 
            IImageObject * This,
            BSTR fileName,
            TIFFCompressionEnum compression);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveToFileJPEG )( 
            IImageObject * This,
            BSTR fileName,
            LONG quality);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveToFileBMP )( 
            IImageObject * This,
            BSTR fileName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveToFilePNG )( 
            IImageObject * This,
            BSTR fileName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ImageViewer )( 
            IImageObject * This,
            /* [retval][out] */ IImageViewer **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ConvertToGrayScale )( 
            IImageObject * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Flip )( 
            IImageObject * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Mirror )( 
            IImageObject * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Negative )( 
            IImageObject * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RotateRight )( 
            IImageObject * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RotateLeft )( 
            IImageObject * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Rotate )( 
            IImageObject * This,
            FLOAT angle,
            InterpolationMethodEnum inMethod,
            OverflowMethodEnum ofMethod,
            VARIANT_BOOL bKeepOriginalSize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Binarize )( 
            IImageObject * This,
            BinarizationMethodEnum method);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSkewAngle )( 
            IImageObject * This,
            FLOAT *angle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Despeckle )( 
            IImageObject * This,
            LONG ksize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Dilate )( 
            IImageObject * This,
            LONG ksize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Erode )( 
            IImageObject * This,
            LONG ksize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsBlank )( 
            IImageObject * This,
            LONG threshold,
            VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ChangeBrightnessContrast )( 
            IImageObject * This,
            LONG brightness,
            LONG contrast);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsGrayScale )( 
            IImageObject * This,
            VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumColors )( 
            IImageObject * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HBITMAP )( 
            IImageObject * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameCount )( 
            IImageObject * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Frame )( 
            IImageObject * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFrame )( 
            IImageObject * This,
            LONG frame);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Size )( 
            IImageObject * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Handle )( 
            IImageObject * This,
            /* [retval][out] */ OLE_HANDLE *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLastErrorText )( 
            IImageObject * This,
            BSTR *errorText);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Save )( 
            IImageObject * This,
            BSTR fileName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Annotations )( 
            IImageObject * This,
            /* [retval][out] */ IAnnotationCollection **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFromBuffer )( 
            IImageObject * This,
            LONG pBuffer,
            LONG width,
            LONG height,
            LONG stride,
            LONG bpp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TooLight )( 
            IImageObject * This,
            LONG threshold,
            VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TooDark )( 
            IImageObject * This,
            LONG threshold,
            VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileName )( 
            IImageObject * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsBlankRect )( 
            IImageObject * This,
            LONG threshold,
            LONG left,
            LONG top,
            LONG right,
            LONG bottom,
            VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Crop )( 
            IImageObject * This,
            LONG left,
            LONG top,
            LONG right,
            LONG bottom,
            IImageObject **image);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFromFile2 )( 
            IImageObject * This,
            BSTR fileName,
            LONG frame,
            BSTR password);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFromMemFile2 )( 
            IImageObject * This,
            SAFEARRAY * *fileData,
            LONG dataLen,
            LONG frame,
            BSTR password);
        
        END_INTERFACE
    } IImageObjectVtbl;

    interface IImageObject
    {
        CONST_VTBL struct IImageObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IImageObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IImageObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IImageObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IImageObject_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IImageObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IImageObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IImageObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IImageObject_get_Width(This,pVal)	\
    ( (This)->lpVtbl -> get_Width(This,pVal) ) 

#define IImageObject_get_Height(This,pVal)	\
    ( (This)->lpVtbl -> get_Height(This,pVal) ) 

#define IImageObject_get_xDPI(This,pVal)	\
    ( (This)->lpVtbl -> get_xDPI(This,pVal) ) 

#define IImageObject_get_yDPI(This,pVal)	\
    ( (This)->lpVtbl -> get_yDPI(This,pVal) ) 

#define IImageObject_LoadFromFile(This,fileName,frame)	\
    ( (This)->lpVtbl -> LoadFromFile(This,fileName,frame) ) 

#define IImageObject_LoadFromBitmap(This,hBmp,hPalette)	\
    ( (This)->lpVtbl -> LoadFromBitmap(This,hBmp,hPalette) ) 

#define IImageObject_LoadFromDIB(This,hDIB)	\
    ( (This)->lpVtbl -> LoadFromDIB(This,hDIB) ) 

#define IImageObject_LoadFromMemFile(This,fileData,dataLen,frame)	\
    ( (This)->lpVtbl -> LoadFromMemFile(This,fileData,dataLen,frame) ) 

#define IImageObject_get_Bpp(This,pVal)	\
    ( (This)->lpVtbl -> get_Bpp(This,pVal) ) 

#define IImageObject_SaveToFileTIFF(This,fileName,compression)	\
    ( (This)->lpVtbl -> SaveToFileTIFF(This,fileName,compression) ) 

#define IImageObject_SaveToFileJPEG(This,fileName,quality)	\
    ( (This)->lpVtbl -> SaveToFileJPEG(This,fileName,quality) ) 

#define IImageObject_SaveToFileBMP(This,fileName)	\
    ( (This)->lpVtbl -> SaveToFileBMP(This,fileName) ) 

#define IImageObject_SaveToFilePNG(This,fileName)	\
    ( (This)->lpVtbl -> SaveToFilePNG(This,fileName) ) 

#define IImageObject_get_ImageViewer(This,pVal)	\
    ( (This)->lpVtbl -> get_ImageViewer(This,pVal) ) 

#define IImageObject_ConvertToGrayScale(This)	\
    ( (This)->lpVtbl -> ConvertToGrayScale(This) ) 

#define IImageObject_Flip(This)	\
    ( (This)->lpVtbl -> Flip(This) ) 

#define IImageObject_Mirror(This)	\
    ( (This)->lpVtbl -> Mirror(This) ) 

#define IImageObject_Negative(This)	\
    ( (This)->lpVtbl -> Negative(This) ) 

#define IImageObject_RotateRight(This)	\
    ( (This)->lpVtbl -> RotateRight(This) ) 

#define IImageObject_RotateLeft(This)	\
    ( (This)->lpVtbl -> RotateLeft(This) ) 

#define IImageObject_Rotate(This,angle,inMethod,ofMethod,bKeepOriginalSize)	\
    ( (This)->lpVtbl -> Rotate(This,angle,inMethod,ofMethod,bKeepOriginalSize) ) 

#define IImageObject_Binarize(This,method)	\
    ( (This)->lpVtbl -> Binarize(This,method) ) 

#define IImageObject_GetSkewAngle(This,angle)	\
    ( (This)->lpVtbl -> GetSkewAngle(This,angle) ) 

#define IImageObject_Despeckle(This,ksize)	\
    ( (This)->lpVtbl -> Despeckle(This,ksize) ) 

#define IImageObject_Dilate(This,ksize)	\
    ( (This)->lpVtbl -> Dilate(This,ksize) ) 

#define IImageObject_Erode(This,ksize)	\
    ( (This)->lpVtbl -> Erode(This,ksize) ) 

#define IImageObject_IsBlank(This,threshold,pVal)	\
    ( (This)->lpVtbl -> IsBlank(This,threshold,pVal) ) 

#define IImageObject_ChangeBrightnessContrast(This,brightness,contrast)	\
    ( (This)->lpVtbl -> ChangeBrightnessContrast(This,brightness,contrast) ) 

#define IImageObject_IsGrayScale(This,pVal)	\
    ( (This)->lpVtbl -> IsGrayScale(This,pVal) ) 

#define IImageObject_get_NumColors(This,pVal)	\
    ( (This)->lpVtbl -> get_NumColors(This,pVal) ) 

#define IImageObject_get_HBITMAP(This,pVal)	\
    ( (This)->lpVtbl -> get_HBITMAP(This,pVal) ) 

#define IImageObject_get_FrameCount(This,pVal)	\
    ( (This)->lpVtbl -> get_FrameCount(This,pVal) ) 

#define IImageObject_get_Frame(This,pVal)	\
    ( (This)->lpVtbl -> get_Frame(This,pVal) ) 

#define IImageObject_SetFrame(This,frame)	\
    ( (This)->lpVtbl -> SetFrame(This,frame) ) 

#define IImageObject_get_Size(This,pVal)	\
    ( (This)->lpVtbl -> get_Size(This,pVal) ) 

#define IImageObject_get_Handle(This,pVal)	\
    ( (This)->lpVtbl -> get_Handle(This,pVal) ) 

#define IImageObject_GetLastErrorText(This,errorText)	\
    ( (This)->lpVtbl -> GetLastErrorText(This,errorText) ) 

#define IImageObject_Save(This,fileName)	\
    ( (This)->lpVtbl -> Save(This,fileName) ) 

#define IImageObject_get_Annotations(This,pVal)	\
    ( (This)->lpVtbl -> get_Annotations(This,pVal) ) 

#define IImageObject_LoadFromBuffer(This,pBuffer,width,height,stride,bpp)	\
    ( (This)->lpVtbl -> LoadFromBuffer(This,pBuffer,width,height,stride,bpp) ) 

#define IImageObject_TooLight(This,threshold,pVal)	\
    ( (This)->lpVtbl -> TooLight(This,threshold,pVal) ) 

#define IImageObject_TooDark(This,threshold,pVal)	\
    ( (This)->lpVtbl -> TooDark(This,threshold,pVal) ) 

#define IImageObject_get_FileName(This,pVal)	\
    ( (This)->lpVtbl -> get_FileName(This,pVal) ) 

#define IImageObject_IsBlankRect(This,threshold,left,top,right,bottom,pVal)	\
    ( (This)->lpVtbl -> IsBlankRect(This,threshold,left,top,right,bottom,pVal) ) 

#define IImageObject_Crop(This,left,top,right,bottom,image)	\
    ( (This)->lpVtbl -> Crop(This,left,top,right,bottom,image) ) 

#define IImageObject_LoadFromFile2(This,fileName,frame,password)	\
    ( (This)->lpVtbl -> LoadFromFile2(This,fileName,frame,password) ) 

#define IImageObject_LoadFromMemFile2(This,fileData,dataLen,frame,password)	\
    ( (This)->lpVtbl -> LoadFromMemFile2(This,fileData,dataLen,frame,password) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IImageObject_INTERFACE_DEFINED__ */


#ifndef __IImageObjectCollection_INTERFACE_DEFINED__
#define __IImageObjectCollection_INTERFACE_DEFINED__

/* interface IImageObjectCollection */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IImageObjectCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EDD8D93F-CFEE-4CD2-A82E-EA8CB26E409E")
    IImageObjectCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            LONG index,
            /* [retval][out] */ IImageObject **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            IImageObject *item) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            IImageObject *item) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveAt( 
            LONG index) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Insert( 
            LONG index,
            IImageObject *item) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveToFileTIFF( 
            BSTR fileName,
            TIFFCompressionEnum compression) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IImageObjectCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IImageObjectCollection * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IImageObjectCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IImageObjectCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IImageObjectCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IImageObjectCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IImageObjectCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IImageObjectCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IImageObjectCollection * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IImageObjectCollection * This,
            LONG index,
            /* [retval][out] */ IImageObject **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IImageObjectCollection * This,
            IImageObject *item);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IImageObjectCollection * This,
            IImageObject *item);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveAt )( 
            IImageObjectCollection * This,
            LONG index);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Insert )( 
            IImageObjectCollection * This,
            LONG index,
            IImageObject *item);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveToFileTIFF )( 
            IImageObjectCollection * This,
            BSTR fileName,
            TIFFCompressionEnum compression);
        
        END_INTERFACE
    } IImageObjectCollectionVtbl;

    interface IImageObjectCollection
    {
        CONST_VTBL struct IImageObjectCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IImageObjectCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IImageObjectCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IImageObjectCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IImageObjectCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IImageObjectCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IImageObjectCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IImageObjectCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IImageObjectCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IImageObjectCollection_get_Item(This,index,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,index,pVal) ) 

#define IImageObjectCollection_Add(This,item)	\
    ( (This)->lpVtbl -> Add(This,item) ) 

#define IImageObjectCollection_Remove(This,item)	\
    ( (This)->lpVtbl -> Remove(This,item) ) 

#define IImageObjectCollection_RemoveAt(This,index)	\
    ( (This)->lpVtbl -> RemoveAt(This,index) ) 

#define IImageObjectCollection_Insert(This,index,item)	\
    ( (This)->lpVtbl -> Insert(This,index,item) ) 

#define IImageObjectCollection_SaveToFileTIFF(This,fileName,compression)	\
    ( (This)->lpVtbl -> SaveToFileTIFF(This,fileName,compression) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IImageObjectCollection_INTERFACE_DEFINED__ */


#ifndef __IImageViewer_INTERFACE_DEFINED__
#define __IImageViewer_INTERFACE_DEFINED__

/* interface IImageViewer */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IImageViewer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("77E33D44-5387-4EB9-AA11-2E193D63999C")
    IImageViewer : public IDispatch
    {
    public:
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_HWND( 
            /* [retval][out] */ LONG_PTR *pHWND) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Image( 
            /* [retval][out] */ IImageObject **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Image( 
            /* [in] */ IImageObject *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Smooth( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Smooth( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AutoSizeMode( 
            /* [retval][out] */ AutoSizeModeEnum *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AutoSizeMode( 
            /* [in] */ AutoSizeModeEnum newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ZoomFactor( 
            /* [retval][out] */ FLOAT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ZoomFactor( 
            /* [in] */ FLOAT newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShowToolbar( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShowToolbar( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResetPages( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ZoomIn( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ZoomOut( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NoImageText( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NoImageText( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IImageViewerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IImageViewer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IImageViewer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IImageViewer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IImageViewer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IImageViewer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IImageViewer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IImageViewer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWND )( 
            IImageViewer * This,
            /* [retval][out] */ LONG_PTR *pHWND);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Image )( 
            IImageViewer * This,
            /* [retval][out] */ IImageObject **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Image )( 
            IImageViewer * This,
            /* [in] */ IImageObject *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Smooth )( 
            IImageViewer * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Smooth )( 
            IImageViewer * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AutoSizeMode )( 
            IImageViewer * This,
            /* [retval][out] */ AutoSizeModeEnum *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AutoSizeMode )( 
            IImageViewer * This,
            /* [in] */ AutoSizeModeEnum newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ZoomFactor )( 
            IImageViewer * This,
            /* [retval][out] */ FLOAT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ZoomFactor )( 
            IImageViewer * This,
            /* [in] */ FLOAT newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Refresh )( 
            IImageViewer * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowToolbar )( 
            IImageViewer * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowToolbar )( 
            IImageViewer * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResetPages )( 
            IImageViewer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ZoomIn )( 
            IImageViewer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ZoomOut )( 
            IImageViewer * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NoImageText )( 
            IImageViewer * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NoImageText )( 
            IImageViewer * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IImageViewerVtbl;

    interface IImageViewer
    {
        CONST_VTBL struct IImageViewerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IImageViewer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IImageViewer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IImageViewer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IImageViewer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IImageViewer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IImageViewer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IImageViewer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IImageViewer_get_HWND(This,pHWND)	\
    ( (This)->lpVtbl -> get_HWND(This,pHWND) ) 

#define IImageViewer_get_Image(This,pVal)	\
    ( (This)->lpVtbl -> get_Image(This,pVal) ) 

#define IImageViewer_put_Image(This,newVal)	\
    ( (This)->lpVtbl -> put_Image(This,newVal) ) 

#define IImageViewer_get_Smooth(This,pVal)	\
    ( (This)->lpVtbl -> get_Smooth(This,pVal) ) 

#define IImageViewer_put_Smooth(This,newVal)	\
    ( (This)->lpVtbl -> put_Smooth(This,newVal) ) 

#define IImageViewer_get_AutoSizeMode(This,pVal)	\
    ( (This)->lpVtbl -> get_AutoSizeMode(This,pVal) ) 

#define IImageViewer_put_AutoSizeMode(This,newVal)	\
    ( (This)->lpVtbl -> put_AutoSizeMode(This,newVal) ) 

#define IImageViewer_get_ZoomFactor(This,pVal)	\
    ( (This)->lpVtbl -> get_ZoomFactor(This,pVal) ) 

#define IImageViewer_put_ZoomFactor(This,newVal)	\
    ( (This)->lpVtbl -> put_ZoomFactor(This,newVal) ) 

#define IImageViewer_Refresh(This)	\
    ( (This)->lpVtbl -> Refresh(This) ) 

#define IImageViewer_get_ShowToolbar(This,pVal)	\
    ( (This)->lpVtbl -> get_ShowToolbar(This,pVal) ) 

#define IImageViewer_put_ShowToolbar(This,newVal)	\
    ( (This)->lpVtbl -> put_ShowToolbar(This,newVal) ) 

#define IImageViewer_ResetPages(This)	\
    ( (This)->lpVtbl -> ResetPages(This) ) 

#define IImageViewer_ZoomIn(This)	\
    ( (This)->lpVtbl -> ZoomIn(This) ) 

#define IImageViewer_ZoomOut(This)	\
    ( (This)->lpVtbl -> ZoomOut(This) ) 

#define IImageViewer_get_NoImageText(This,pVal)	\
    ( (This)->lpVtbl -> get_NoImageText(This,pVal) ) 

#define IImageViewer_put_NoImageText(This,newVal)	\
    ( (This)->lpVtbl -> put_NoImageText(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IImageViewer_INTERFACE_DEFINED__ */


#ifndef __IAnnotationLine_INTERFACE_DEFINED__
#define __IAnnotationLine_INTERFACE_DEFINED__

/* interface IAnnotationLine */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IAnnotationLine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BC822C0B-1801-40CF-BC4D-D298C48F65C8")
    IAnnotationLine : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_x1( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_x1( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_y1( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_y1( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_x2( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_x2( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_y2( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_y2( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Color( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Color( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Frame( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Frame( 
            /* [in] */ LONG newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAnnotationLineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAnnotationLine * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAnnotationLine * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAnnotationLine * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAnnotationLine * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAnnotationLine * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAnnotationLine * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAnnotationLine * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_x1 )( 
            IAnnotationLine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_x1 )( 
            IAnnotationLine * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_y1 )( 
            IAnnotationLine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_y1 )( 
            IAnnotationLine * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_x2 )( 
            IAnnotationLine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_x2 )( 
            IAnnotationLine * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_y2 )( 
            IAnnotationLine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_y2 )( 
            IAnnotationLine * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Color )( 
            IAnnotationLine * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Color )( 
            IAnnotationLine * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Frame )( 
            IAnnotationLine * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Frame )( 
            IAnnotationLine * This,
            /* [in] */ LONG newVal);
        
        END_INTERFACE
    } IAnnotationLineVtbl;

    interface IAnnotationLine
    {
        CONST_VTBL struct IAnnotationLineVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAnnotationLine_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAnnotationLine_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAnnotationLine_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAnnotationLine_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAnnotationLine_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAnnotationLine_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAnnotationLine_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IAnnotationLine_get_x1(This,pVal)	\
    ( (This)->lpVtbl -> get_x1(This,pVal) ) 

#define IAnnotationLine_put_x1(This,newVal)	\
    ( (This)->lpVtbl -> put_x1(This,newVal) ) 

#define IAnnotationLine_get_y1(This,pVal)	\
    ( (This)->lpVtbl -> get_y1(This,pVal) ) 

#define IAnnotationLine_put_y1(This,newVal)	\
    ( (This)->lpVtbl -> put_y1(This,newVal) ) 

#define IAnnotationLine_get_x2(This,pVal)	\
    ( (This)->lpVtbl -> get_x2(This,pVal) ) 

#define IAnnotationLine_put_x2(This,newVal)	\
    ( (This)->lpVtbl -> put_x2(This,newVal) ) 

#define IAnnotationLine_get_y2(This,pVal)	\
    ( (This)->lpVtbl -> get_y2(This,pVal) ) 

#define IAnnotationLine_put_y2(This,newVal)	\
    ( (This)->lpVtbl -> put_y2(This,newVal) ) 

#define IAnnotationLine_get_Color(This,pVal)	\
    ( (This)->lpVtbl -> get_Color(This,pVal) ) 

#define IAnnotationLine_put_Color(This,newVal)	\
    ( (This)->lpVtbl -> put_Color(This,newVal) ) 

#define IAnnotationLine_get_Frame(This,pVal)	\
    ( (This)->lpVtbl -> get_Frame(This,pVal) ) 

#define IAnnotationLine_put_Frame(This,newVal)	\
    ( (This)->lpVtbl -> put_Frame(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAnnotationLine_INTERFACE_DEFINED__ */


#ifndef __IAnnotationRectangle_INTERFACE_DEFINED__
#define __IAnnotationRectangle_INTERFACE_DEFINED__

/* interface IAnnotationRectangle */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IAnnotationRectangle;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1BEFB84E-E7F1-42AE-9A5B-296CEEC1843C")
    IAnnotationRectangle : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Left( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Top( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Right( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Right( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Bottom( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Bottom( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Color( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Color( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Frame( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Frame( 
            /* [in] */ LONG newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAnnotationRectangleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAnnotationRectangle * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAnnotationRectangle * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAnnotationRectangle * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAnnotationRectangle * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAnnotationRectangle * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAnnotationRectangle * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAnnotationRectangle * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IAnnotationRectangle * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IAnnotationRectangle * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IAnnotationRectangle * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IAnnotationRectangle * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Right )( 
            IAnnotationRectangle * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Right )( 
            IAnnotationRectangle * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Bottom )( 
            IAnnotationRectangle * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Bottom )( 
            IAnnotationRectangle * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Color )( 
            IAnnotationRectangle * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Color )( 
            IAnnotationRectangle * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Frame )( 
            IAnnotationRectangle * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Frame )( 
            IAnnotationRectangle * This,
            /* [in] */ LONG newVal);
        
        END_INTERFACE
    } IAnnotationRectangleVtbl;

    interface IAnnotationRectangle
    {
        CONST_VTBL struct IAnnotationRectangleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAnnotationRectangle_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAnnotationRectangle_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAnnotationRectangle_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAnnotationRectangle_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAnnotationRectangle_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAnnotationRectangle_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAnnotationRectangle_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IAnnotationRectangle_get_Left(This,pVal)	\
    ( (This)->lpVtbl -> get_Left(This,pVal) ) 

#define IAnnotationRectangle_put_Left(This,newVal)	\
    ( (This)->lpVtbl -> put_Left(This,newVal) ) 

#define IAnnotationRectangle_get_Top(This,pVal)	\
    ( (This)->lpVtbl -> get_Top(This,pVal) ) 

#define IAnnotationRectangle_put_Top(This,newVal)	\
    ( (This)->lpVtbl -> put_Top(This,newVal) ) 

#define IAnnotationRectangle_get_Right(This,pVal)	\
    ( (This)->lpVtbl -> get_Right(This,pVal) ) 

#define IAnnotationRectangle_put_Right(This,newVal)	\
    ( (This)->lpVtbl -> put_Right(This,newVal) ) 

#define IAnnotationRectangle_get_Bottom(This,pVal)	\
    ( (This)->lpVtbl -> get_Bottom(This,pVal) ) 

#define IAnnotationRectangle_put_Bottom(This,newVal)	\
    ( (This)->lpVtbl -> put_Bottom(This,newVal) ) 

#define IAnnotationRectangle_get_Color(This,pVal)	\
    ( (This)->lpVtbl -> get_Color(This,pVal) ) 

#define IAnnotationRectangle_put_Color(This,newVal)	\
    ( (This)->lpVtbl -> put_Color(This,newVal) ) 

#define IAnnotationRectangle_get_Frame(This,pVal)	\
    ( (This)->lpVtbl -> get_Frame(This,pVal) ) 

#define IAnnotationRectangle_put_Frame(This,newVal)	\
    ( (This)->lpVtbl -> put_Frame(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAnnotationRectangle_INTERFACE_DEFINED__ */


#ifndef __ITIFFUtils_INTERFACE_DEFINED__
#define __ITIFFUtils_INTERFACE_DEFINED__

/* interface ITIFFUtils */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ITIFFUtils;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("66E408AF-3D0F-41A8-BA4E-0EEDE82ACE94")
    ITIFFUtils : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SplitByPage( 
            BSTR sourceFileName,
            LONG pageNumber,
            BSTR destFileName1,
            BSTR destFileName2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SplitByPages( 
            BSTR sourceFileName,
            BSTR nameTemplate,
            BSTR pageSequenceStr) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SplitToPortions( 
            BSTR sourceFileName,
            BSTR nameTemplate,
            LONG portionSize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Merge( 
            BSTR fileName1,
            BSTR fileName2,
            BSTR outputFileName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeletePage( 
            BSTR fileName,
            LONG pageNumber) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SplitByPageBuffer( 
            SAFEARRAY * *sourceFileData,
            LONG pageNumber,
            SAFEARRAY * *destFileData1,
            SAFEARRAY * *destFileData2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MergeBuffer( 
            SAFEARRAY * *fileData1,
            SAFEARRAY * *fileData2,
            SAFEARRAY * *outputFileData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeletePageBuffer( 
            SAFEARRAY * *fileData,
            LONG pageNumber) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITIFFUtilsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITIFFUtils * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITIFFUtils * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITIFFUtils * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITIFFUtils * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITIFFUtils * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITIFFUtils * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITIFFUtils * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SplitByPage )( 
            ITIFFUtils * This,
            BSTR sourceFileName,
            LONG pageNumber,
            BSTR destFileName1,
            BSTR destFileName2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SplitByPages )( 
            ITIFFUtils * This,
            BSTR sourceFileName,
            BSTR nameTemplate,
            BSTR pageSequenceStr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SplitToPortions )( 
            ITIFFUtils * This,
            BSTR sourceFileName,
            BSTR nameTemplate,
            LONG portionSize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Merge )( 
            ITIFFUtils * This,
            BSTR fileName1,
            BSTR fileName2,
            BSTR outputFileName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeletePage )( 
            ITIFFUtils * This,
            BSTR fileName,
            LONG pageNumber);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SplitByPageBuffer )( 
            ITIFFUtils * This,
            SAFEARRAY * *sourceFileData,
            LONG pageNumber,
            SAFEARRAY * *destFileData1,
            SAFEARRAY * *destFileData2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MergeBuffer )( 
            ITIFFUtils * This,
            SAFEARRAY * *fileData1,
            SAFEARRAY * *fileData2,
            SAFEARRAY * *outputFileData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeletePageBuffer )( 
            ITIFFUtils * This,
            SAFEARRAY * *fileData,
            LONG pageNumber);
        
        END_INTERFACE
    } ITIFFUtilsVtbl;

    interface ITIFFUtils
    {
        CONST_VTBL struct ITIFFUtilsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITIFFUtils_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITIFFUtils_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITIFFUtils_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITIFFUtils_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITIFFUtils_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITIFFUtils_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITIFFUtils_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITIFFUtils_SplitByPage(This,sourceFileName,pageNumber,destFileName1,destFileName2)	\
    ( (This)->lpVtbl -> SplitByPage(This,sourceFileName,pageNumber,destFileName1,destFileName2) ) 

#define ITIFFUtils_SplitByPages(This,sourceFileName,nameTemplate,pageSequenceStr)	\
    ( (This)->lpVtbl -> SplitByPages(This,sourceFileName,nameTemplate,pageSequenceStr) ) 

#define ITIFFUtils_SplitToPortions(This,sourceFileName,nameTemplate,portionSize)	\
    ( (This)->lpVtbl -> SplitToPortions(This,sourceFileName,nameTemplate,portionSize) ) 

#define ITIFFUtils_Merge(This,fileName1,fileName2,outputFileName)	\
    ( (This)->lpVtbl -> Merge(This,fileName1,fileName2,outputFileName) ) 

#define ITIFFUtils_DeletePage(This,fileName,pageNumber)	\
    ( (This)->lpVtbl -> DeletePage(This,fileName,pageNumber) ) 

#define ITIFFUtils_SplitByPageBuffer(This,sourceFileData,pageNumber,destFileData1,destFileData2)	\
    ( (This)->lpVtbl -> SplitByPageBuffer(This,sourceFileData,pageNumber,destFileData1,destFileData2) ) 

#define ITIFFUtils_MergeBuffer(This,fileData1,fileData2,outputFileData)	\
    ( (This)->lpVtbl -> MergeBuffer(This,fileData1,fileData2,outputFileData) ) 

#define ITIFFUtils_DeletePageBuffer(This,fileData,pageNumber)	\
    ( (This)->lpVtbl -> DeletePageBuffer(This,fileData,pageNumber) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITIFFUtils_INTERFACE_DEFINED__ */


#ifndef __ITwain_INTERFACE_DEFINED__
#define __ITwain_INTERFACE_DEFINED__

/* interface ITwain */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ITwain;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("502F44F6-FADA-42AD-8E94-58DBC0B45A30")
    ITwain : public IDispatch
    {
    public:
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_HWND( 
            /* [retval][out] */ LONG_PTR *pHWND) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Acquire( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DeviceCount( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DeviceName( 
            LONG index,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentDeviceName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurrentDeviceName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentDeviceIndex( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurrentDeviceIndex( 
            /* [in] */ LONG newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITwainVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITwain * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITwain * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITwain * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITwain * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITwain * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITwain * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITwain * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWND )( 
            ITwain * This,
            /* [retval][out] */ LONG_PTR *pHWND);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Acquire )( 
            ITwain * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeviceCount )( 
            ITwain * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeviceName )( 
            ITwain * This,
            LONG index,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentDeviceName )( 
            ITwain * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurrentDeviceName )( 
            ITwain * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentDeviceIndex )( 
            ITwain * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurrentDeviceIndex )( 
            ITwain * This,
            /* [in] */ LONG newVal);
        
        END_INTERFACE
    } ITwainVtbl;

    interface ITwain
    {
        CONST_VTBL struct ITwainVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITwain_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITwain_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITwain_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITwain_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITwain_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITwain_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITwain_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITwain_get_HWND(This,pHWND)	\
    ( (This)->lpVtbl -> get_HWND(This,pHWND) ) 

#define ITwain_Acquire(This)	\
    ( (This)->lpVtbl -> Acquire(This) ) 

#define ITwain_get_DeviceCount(This,pVal)	\
    ( (This)->lpVtbl -> get_DeviceCount(This,pVal) ) 

#define ITwain_get_DeviceName(This,index,pVal)	\
    ( (This)->lpVtbl -> get_DeviceName(This,index,pVal) ) 

#define ITwain_get_CurrentDeviceName(This,pVal)	\
    ( (This)->lpVtbl -> get_CurrentDeviceName(This,pVal) ) 

#define ITwain_put_CurrentDeviceName(This,newVal)	\
    ( (This)->lpVtbl -> put_CurrentDeviceName(This,newVal) ) 

#define ITwain_get_CurrentDeviceIndex(This,pVal)	\
    ( (This)->lpVtbl -> get_CurrentDeviceIndex(This,pVal) ) 

#define ITwain_put_CurrentDeviceIndex(This,newVal)	\
    ( (This)->lpVtbl -> put_CurrentDeviceIndex(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITwain_INTERFACE_DEFINED__ */


#ifndef __IPDFUtils_INTERFACE_DEFINED__
#define __IPDFUtils_INTERFACE_DEFINED__

/* interface IPDFUtils */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IPDFUtils;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8512900F-3D05-49E2-8BCC-0FBF1CE737CA")
    IPDFUtils : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SplitByPage( 
            BSTR sourceFileName,
            LONG pageNumber,
            BSTR destFileName1,
            BSTR destFileName2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SplitByPageBuffer( 
            SAFEARRAY * *sourceFileData,
            LONG pageNumber,
            SAFEARRAY * *destFileData1,
            SAFEARRAY * *destFileData2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Merge( 
            BSTR fileName1,
            BSTR fileName2,
            BSTR outputFileName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MergeBuffer( 
            SAFEARRAY * *fileData1,
            SAFEARRAY * *fileData2,
            SAFEARRAY * *outputFileData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeletePage( 
            BSTR fileName,
            LONG pageNumber) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeletePageBuffer( 
            SAFEARRAY * *fileData,
            LONG pageNumber) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPDFUtilsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPDFUtils * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPDFUtils * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPDFUtils * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPDFUtils * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPDFUtils * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPDFUtils * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPDFUtils * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SplitByPage )( 
            IPDFUtils * This,
            BSTR sourceFileName,
            LONG pageNumber,
            BSTR destFileName1,
            BSTR destFileName2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SplitByPageBuffer )( 
            IPDFUtils * This,
            SAFEARRAY * *sourceFileData,
            LONG pageNumber,
            SAFEARRAY * *destFileData1,
            SAFEARRAY * *destFileData2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Merge )( 
            IPDFUtils * This,
            BSTR fileName1,
            BSTR fileName2,
            BSTR outputFileName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MergeBuffer )( 
            IPDFUtils * This,
            SAFEARRAY * *fileData1,
            SAFEARRAY * *fileData2,
            SAFEARRAY * *outputFileData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeletePage )( 
            IPDFUtils * This,
            BSTR fileName,
            LONG pageNumber);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeletePageBuffer )( 
            IPDFUtils * This,
            SAFEARRAY * *fileData,
            LONG pageNumber);
        
        END_INTERFACE
    } IPDFUtilsVtbl;

    interface IPDFUtils
    {
        CONST_VTBL struct IPDFUtilsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPDFUtils_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPDFUtils_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPDFUtils_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPDFUtils_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPDFUtils_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPDFUtils_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPDFUtils_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IPDFUtils_SplitByPage(This,sourceFileName,pageNumber,destFileName1,destFileName2)	\
    ( (This)->lpVtbl -> SplitByPage(This,sourceFileName,pageNumber,destFileName1,destFileName2) ) 

#define IPDFUtils_SplitByPageBuffer(This,sourceFileData,pageNumber,destFileData1,destFileData2)	\
    ( (This)->lpVtbl -> SplitByPageBuffer(This,sourceFileData,pageNumber,destFileData1,destFileData2) ) 

#define IPDFUtils_Merge(This,fileName1,fileName2,outputFileName)	\
    ( (This)->lpVtbl -> Merge(This,fileName1,fileName2,outputFileName) ) 

#define IPDFUtils_MergeBuffer(This,fileData1,fileData2,outputFileData)	\
    ( (This)->lpVtbl -> MergeBuffer(This,fileData1,fileData2,outputFileData) ) 

#define IPDFUtils_DeletePage(This,fileName,pageNumber)	\
    ( (This)->lpVtbl -> DeletePage(This,fileName,pageNumber) ) 

#define IPDFUtils_DeletePageBuffer(This,fileData,pageNumber)	\
    ( (This)->lpVtbl -> DeletePageBuffer(This,fileData,pageNumber) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPDFUtils_INTERFACE_DEFINED__ */



#ifndef __DTKImageLib_LIBRARY_DEFINED__
#define __DTKImageLib_LIBRARY_DEFINED__

/* library DTKImageLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DTKImageLib;

EXTERN_C const CLSID CLSID_ImageObject;

#ifdef __cplusplus

class DECLSPEC_UUID("FB592E29-EF46-4BBA-A15C-8249ECD70AE9")
ImageObject;
#endif

EXTERN_C const CLSID CLSID_ImageObjectCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("2A4DB1F5-4B2E-4C88-A9A6-F84FAC49A73A")
ImageObjectCollection;
#endif

#ifndef ___IImageViewerEvents_DISPINTERFACE_DEFINED__
#define ___IImageViewerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IImageViewerEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IImageViewerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5055B371-3B77-4BFB-BCA4-676A0C73892D")
    _IImageViewerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IImageViewerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IImageViewerEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IImageViewerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IImageViewerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IImageViewerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IImageViewerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IImageViewerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IImageViewerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IImageViewerEventsVtbl;

    interface _IImageViewerEvents
    {
        CONST_VTBL struct _IImageViewerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IImageViewerEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IImageViewerEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IImageViewerEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IImageViewerEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IImageViewerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IImageViewerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IImageViewerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IImageViewerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ImageViewer;

#ifdef __cplusplus

class DECLSPEC_UUID("19DF1950-F74D-4834-BAC7-8972BD5A9EF4")
ImageViewer;
#endif

EXTERN_C const CLSID CLSID_AnnotationLine;

#ifdef __cplusplus

class DECLSPEC_UUID("94391FF5-748D-4544-96FF-BAA168CC631C")
AnnotationLine;
#endif

EXTERN_C const CLSID CLSID_AnnotationCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("1D0FA59D-A405-4DC5-93B4-1B3EFAEC6D4A")
AnnotationCollection;
#endif

EXTERN_C const CLSID CLSID_AnnotationRectangle;

#ifdef __cplusplus

class DECLSPEC_UUID("F10AF0E9-626C-4B13-A24B-B2A629E5C010")
AnnotationRectangle;
#endif

#ifndef ___ITIFFUtilsEvents_DISPINTERFACE_DEFINED__
#define ___ITIFFUtilsEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ITIFFUtilsEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ITIFFUtilsEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C6F7E45B-4095-4895-B96C-D3BDA8C1A6D4")
    _ITIFFUtilsEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ITIFFUtilsEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ITIFFUtilsEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ITIFFUtilsEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ITIFFUtilsEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ITIFFUtilsEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ITIFFUtilsEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ITIFFUtilsEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ITIFFUtilsEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ITIFFUtilsEventsVtbl;

    interface _ITIFFUtilsEvents
    {
        CONST_VTBL struct _ITIFFUtilsEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ITIFFUtilsEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ITIFFUtilsEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ITIFFUtilsEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ITIFFUtilsEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ITIFFUtilsEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ITIFFUtilsEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ITIFFUtilsEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ITIFFUtilsEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_TIFFUtils;

#ifdef __cplusplus

class DECLSPEC_UUID("85EE5D41-C409-429A-87AB-B2E3062B638C")
TIFFUtils;
#endif

#ifndef ___IPDFUtilsEvents_DISPINTERFACE_DEFINED__
#define ___IPDFUtilsEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IPDFUtilsEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IPDFUtilsEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D502E946-301C-4232-AD74-F301B107D00E")
    _IPDFUtilsEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IPDFUtilsEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IPDFUtilsEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IPDFUtilsEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IPDFUtilsEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IPDFUtilsEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IPDFUtilsEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IPDFUtilsEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IPDFUtilsEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IPDFUtilsEventsVtbl;

    interface _IPDFUtilsEvents
    {
        CONST_VTBL struct _IPDFUtilsEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IPDFUtilsEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IPDFUtilsEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IPDFUtilsEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IPDFUtilsEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IPDFUtilsEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IPDFUtilsEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IPDFUtilsEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IPDFUtilsEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PDFUtils;

#ifdef __cplusplus

class DECLSPEC_UUID("20500A16-DCFE-495D-B457-D7DD2DCF6A17")
PDFUtils;
#endif

#ifndef ___ITwainEvents_DISPINTERFACE_DEFINED__
#define ___ITwainEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ITwainEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ITwainEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6098AD6D-1A23-49B5-AACC-28AE1929DCAA")
    _ITwainEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ITwainEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ITwainEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ITwainEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ITwainEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ITwainEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ITwainEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ITwainEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ITwainEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ITwainEventsVtbl;

    interface _ITwainEvents
    {
        CONST_VTBL struct _ITwainEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ITwainEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ITwainEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ITwainEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ITwainEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ITwainEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ITwainEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ITwainEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ITwainEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Twain;

#ifdef __cplusplus

class DECLSPEC_UUID("E01EC6D2-86F6-4276-9AB3-8764729310C1")
Twain;
#endif
#endif /* __DTKImageLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


