/******************************************************************************
 *
 * The copyright in this software is owned by Rockchip and/or its licensors.
 * This software is made available subject to the conditions of the license 
 * terms to be determined and negotiated by Rockchip and you.
 * THIS SOFTWARE IS PROVIDED TO YOU ON AN "AS IS" BASIS and ROCKCHP AND/OR 
 * ITS LICENSORS DISCLAIMS ANY AND ALL WARRANTIES AND REPRESENTATIONS WITH 
 * RESPECT TO SUCH SOFTWARE, WHETHER EXPRESS,IMPLIED, STATUTORY OR OTHERWISE, 
 * INCLUDING WITHOUT LIMITATION, ANY IMPLIED WARRANTIES OF TITLE, NON-INFRINGEMENT, 
 * MERCHANTABILITY, SATISFACTROY QUALITY, ACCURACY OR FITNESS FOR A PARTICULAR PURPOSE. 
 * Except as expressively authorized by Rockchip and/or its licensors, you may not 
 * (a) disclose, distribute, sell, sub-license, or transfer this software to any third party, 
 * in whole or part; (b) modify this software, in whole or part; (c) decompile, reverse-engineer, 
 * dissemble, or attempt to derive any source code from the software.
 *
 *****************************************************************************/
/**
 * @file cam_engine_interface.h
 *
 * @brief
 *   Cam Engine C++ API.
 *
 *****************************************************************************/
/**
 * @page module_name_page Module Name
 * Describe here what this module does.
 *
 * For a detailed list of functions and implementation detail refer to:
 * - @ref module_name
 *
 * @defgroup module_name Module Name
 * @{
 *
 */

#ifndef __CAM_ENGINE_ITF_H__
#define __CAM_ENGINE_ITF_H__

#include <isi/isi.h>
#include <isi/isi_iss.h>

// calibration database
#include <cam_calibdb/cam_calibdb_api.h>

// cam-engine
#include <cam_engine/cam_engine_api.h>
#include <cam_engine/cam_engine_drv_api.h>
#include <cam_engine/cam_engine_aaa_api.h>
#include <cam_engine/cam_engine_jpe_api.h>
#include <cam_engine/cam_engine_mi_api.h>
#include <cam_engine/cam_engine_isp_api.h>
#include <cam_engine/cam_engine_imgeffects_api.h>
#include <cam_engine/cam_engine_cproc_api.h>
#include <cam_engine/cam_engine_simp_api.h>
#include <common/cam_info.h>

#include <vector>

#include <CameraHal_board_xml_parse.h>
#include <linux/version.h>

/*
*              SILICONIMAGE LIBISP VERSION NOTE
*
*v0.2.0x00 : add checkAfShot and registerAfEvtQue api, log level;
*v0.3.0x00 : add configFlash/startFlash/stopFlash
*v0.4.0x00 : invalidate DominateIlluProfileIdx init in AwbIlluEstProcessFrame, because AwbReConfigure maybe failed
*            if awb fLikeHoodSum near zero;
*v0.5.0x00 : 1). if f_s < 0.25 and d65 weight < 0.75, illuminate d65 -> cwf; 
*            2). f_RgProj min is 1.3, if illuminate is no A; 
*v0.6.0x00 :
             1) support mi output yuv420 when sensor output yuv422
             2) merge cameric_mi.c from isp vendor version delivery_sw_M14_v2_rel_1_2_20140424
*v0.7.0:
*            1). AfCntMax 120 -> 10 in  CamEngineCamerIcDrvMeasureCbRestart, because af validate delay too long after changeResoultion
*            2). flash delay val 100000 -> 700000;
*v0.8.0:     1). can change mp config when streaming , could do digital zoom use this feature.
*v0.9.0:     1). soc sensor buswidth depend on boad XML
*v0.a.0:
*            this version sync v0.7.1 version;
*            1). flash delay val 70000 -> 50000; 
*v0.b.0:     1). optimize flash unit
*v0.c.0:     1) fix i2c read error
*
*v0.d.0:  support ov8858 and ov13850 sensor dirver
*v0.e.0:   1) add flash trig pol control
*v0.f.0:   1) iomux trigger pin as flash_trigger_out when do flash,or as gpio.
*v0.g.0:   1) increase fl_time
*v0.0x10.0:
*          1) isp_flash_prediv is wrong ,fix it
*v0.0x11.0:
*          1) marvin reset(ircl bit7) may be hold ahb when isp_clk:isp_hclk > 2:1, so used hard reset (cru reset);
*v0.0x12.0:
*          1) support flash auto mode
*v0.0x13.0:
*          1) maybe block by stop some times,add timeout for this.
*v0.0x14.0:
*          1) add interface getAwbGainInfo/getIlluEstInfo.  
*v0.0x15.0:
*          1) modify af(speed up), but continues af also may be failed;
           1) maybe block by stop some times,add timeout for this.
*v0.0x16.0:
*          1) when data loss or pic size erro occure,frame is corrupted,so discard this frame(not work well for 2 lanes now).
*v0.0x17.0:
*          1) add interface getSensorXmlVersion.
*v0.0x18.0:
*          1) fix afStop may deadlock for queue is full;
*          2) fix afOneShot cmd may be lost if vcm move time > 1 frames;
*          3) fix IsiMdiSetupMotoDrive maxfocus error in opensensor for isi v0.3.0
*v0.0x19.0:
*          1) set mi burst to 16,reduce the probability of data loss caused by ddr bandwidth
*          2) optimize timeout procedure for CAM_ENGINE_CMD_HW_STREAMING_FINISHED
*v0.0x20.0:
*          1) af add support fine seach;
*          2) af add support soc sensor af;
*v0.0x21.0:
*          1) add support mipi soc, add ov2685;
*          2) add ov5640;
*v0.0x22.0:
*          1) add flash enable pol control.
*          2) set FLASH_SKIP_FRAMES 3 for P4 case.
*v0.0x23.0:
*          1) verify if buffer has been fullfilled by isp (may be wrong mi frame_end irq ?)or not , if not ,drop it.
*v0.0x24.0:
*          1) add awb yellow and change d65 ill to cwf parament in tuning xml   
*v0.0x25.0:
*          1) set PIC_BUFFER_NUM_MAIN_SENSOR to 4, and set PIC_BUFFER_SIZE_MAIN_SENSOR to 30M for 1300M raw picture size
*          2) may dead while on MediaBufPoolGetBuffer due to that some buffer may not freed by app,so reset buffer pool when stop preview.
*v0.0x26.0:
*          1) add sensor drv version
*v0.0x27.0:
*          1) add chkAwbIllumination api;
*v0.0x28.0:
*          1) add trace information in AfProcessFrame when result is RET_PENDING;
*v0.0x29.0:
*          1) add lockAec api;
*          2) fix af search fine path distance is too short, af pos may be same, so AfProcessFrame return RET_PENDING;
*v0.0x2a.0:
*          1) support iommu;
*v0.0x2b.0:
*          1) add awb stable
*v0.0x2c.0:
*          1) limit MaxGain/MaxExposuretime in mannual exposure function.
*v0.0x2d.0:
*          1) modify getPreferedSensorRes and getSensorCaps api;
*          2) modify resolution macro ISI_RES_XXXX;
*v0.0x2e.0:
*          1) Resolution which pixels is large than 90% pixels request, this is ok resolution in getPreferedSensorRes;
*v0.0x2f.0:
*          1) fix isi.h version macro is error, switch to v0.7.0
*v1.0.0:
*          1) Reuse buffer if the buffer haven't been fill image data instead of drop it;
*v1.1.0:
*          1) calibdb modify to parse tab and space error
*v1.2.0:
*          1) fix mi CbCr buffer address must align which is define by c_burstlength;
*v1.3.0:
*          1) calibdb modify to parse NiYang xml,and add log  
*v1.4.0:
*          1) add setAecMeasureWindow/setAfMeasureWindow/getAfMeasureWindow api;
*v1.5.0:
*          1) fix continues af may be trigger when machine is moving; Continues af must be triggered after many frames 
*              sharpness diffrence is < 0.02.
*v1.6.0:
*          1) support detect measure window is changed or not in continue af; Trige af if it is changed;
*v1.7.0:
*          1) add setIspBufferInfo api to set isp buffer num and size;
*v1.8.0:
*          1) ae and histogram support AverageMeter and CenterWeightMeter; touch ae used AverageMeter, preview ae used CenterWeightMeter;
*          2) AfProcessFrame run after check AecSetted when receive CAMERIC_ISP_EVENT_AFM;
*v1.9.0:
*          1) support change framerate in CamEngineItf::changeResolution;
*v1.0xa.0:
*          1) Change vcm position to MaxFocus when af failed;
*          2) AF is failed when sharpness < 300;
*          3) AF fine search is not run, fix it;
*          4) AF messure window <= 200x200 for measure result will overflow;
*v1.0x0b.0:
*          1) record cwf and d65 ill index, because d65 may be switch to cwf in awb for lens;
*          2) Check sharpness < 300 , and change vcm position to MaxFocus when tracking;
*v1.0x0c.0:
*          1) add support request_res_exp in getPreferedSensorRes;
*v1.0x0d.0:
*          1) getSensorXmlVersion func may cause array bound exceeded,fixed it.
*v1.0x0e.0:
*          1) request_res_exp -> request_exp_t in getPreferedSensorRes;
*v1.0x0f.0:
*          1) add ISI_RES_1296_972P10/ISI_RES_1296_972P20/ISI_RES_1296_972P25 in mapcaps.cpp.
*v1.0x10.0:
*          1) Check sharpness < 500 , and change vcm position to MaxFocus when tracking;
*          2) Change weight for CenterWeightMeter ae and histom;
*v1.0x10.1:
*          1) fix for MonkeyTest.
*v1.0x11.0:
*          1) Change vcm position to MaxFocus when af failed, this operate in AfSearching end;
*v1.0x12.0:
*    include  for ifive(hisense f415)
*    v1.0x11.1:
*          1) invalidate AfProcessFrame must check AecSettled;
*          2) pAecCtx->AfpsCurrResolution obtain real resolution from sensor driver,because pAecCtx->AfpsCurrResolution
*             may be error;
*    1) add support 3a lock and unlock api;
*v1.0x13.0
*    include for ifive(hisense f415)
*    v1.0x11.2:
*          1) add g_pCamEngineCtx_lock for g_pCamEngineCtx;
*v1.0x14.0
*          1) support ext flash
*v1.0x15.0
*    include for ifive(hisense f415)
*    v1.0x11.3:
*          1) pAecCtx->EcmT0fac(1.0 -> 13.0) for ISI_RES_2592_1944P7 and ISI_RES_3264_2448P7;
*          2) AecReConfigure must call EcmExecuteDirect, no call EcmExecute;  
*    v1.0x11.4:
*          1) getPreferedSensorRes support max_exp_res/max_fps_res/max_res for different request resolution;
*    v1.0x11.5:
*          1) pAecCtx->EcmT0 define max integration time for 5M and 8M resolution;
*v1.0x16.0:
*   1)  compatible with android 5.0 .
*v1.0x17.0:
*    include for ifive(hisense f415)
*    v1.0x11.6:
*          1) if enableAfps is false, change framerate only is invalidate in changeResolution;
*v1.0x18.0:
    1) wrong of operation of comparation 0.0 to  float in func getPreferedSensorRes,fix it.
*v1.0x19.0:
    1) correct ion heap for allocate ,replace ion_handle* with ion_user_handle_t
*v1.0x20.0:
    1) wrong of operation of comparation dev_mask in func HalFlashTrigCtrl,fix it.
*v1.0x21.0:
    1) add to support 5040 afps.
*v1.0x22.0:
    1) support sensor otp i2c info
*v1.0x23.0:
    1)  compatible with ion no matter new or old version.
*v1.0x24.0:
    1)  change isp output from limit range to full range
*v1.0x25.0:
    1)  modify android 5.x compile condition. 
*v1.0x26.0:
    1)  fix some aec ecm flicker issues
*v1.0x27.0:
    1) support 32bit andr 64bit.
*v1.0x28.0:
    1) add callback func param sensor_version_get_p in callback func IsiCheckOTPInfo_t.
*v1.0x29.0:
    1) support sensor powerup sequence configurable.
*v1.0x30.0:
    1) add enableSensorOTP and setupOTPInfo interfaces.
*v1.0x31.0:
    1) compatible with android 6.0
*v1.0x32.0:
    1) compatible with android 6.0 64bit  
*v1.0x33.0:
    1) add 0 cc offset in lowlight sence     //oyyf 
*v1.0x34.0:
    1) open camsys_marvin or camsys_marvin1 depend on phy config 
*v1.0x35.0:
    1) fix something bug when open camsys_marvin1
*v1.0x35.1:
    1) protect variables EcmT0 and Tmax in ecm.c.
    2) modify HUGE_VAL_GAIN from 0xffffffff to 65535.
    3) modify expresion 'floorf' to 'roundf' in 'Tint=MIN(Tmax,roundf(Tc/pAecCtx->EcmTflicker)*pAecCtx->EcmTflicker)'.
    4) support 3120x3120P30.
*v1.0x36.0:
    1) IMX214 ID address:0x1617 ID num:214.
    2) OV13850 support r1a and r2a.
    3) support 4224_3120P15\3120_3120P15\3120_3120P25\2112_1560P30.
*v1.0x37.0:
    1) I2C_NUM up to six.
    2) support frame rates limition set when preview. 
*v1.0x38.0:
    1) iFive_F516 IMX214 tuning. 
*v1.0x39.0:
    1) support two cameras preview at the same time.
*v1.0x40.0:
    1) support limit frame rate.
*v1.0x41.0:
    1) support android Nougat.
*v1.0x42.0:
    1) Gammaout can config in IQ xml.
    2) AEC/Histogram region weights can config in IQ xml.
    3) AEC measuring window can config in IQ xml.
    4) Denosise/sharpen level can config under different gains in IQ xml.
    5) AEC exposure curve can config in IQ xml.
    6) AWB measuring window can config in IQ xml.
*v1.0x43.0:
    1) fix ion handle compile error when compatible with android marshallow and nougat.
*v1.0x44.0:
    1) fix the bug of AEC\AWB\Denosise\sharpen IQ xml parameter config.
    2) fix the bug that capture can't get right integral time information.
    3) support get the sensor color from sensor drive.
*v1.0x45.0:
    1) UVNR can config in IQ xml.
    2) support 3DNR function and can config in IQ xml.
*v1.0x46.0:
    1) memory allocation interface can be set outside through struct HalPara_s
*v1.0x47.0:
    1) support CprocConfig and ConvRange distinguish between preview and capture.
    2) IMX214 tuning file.
    3) cam_board.xml update.
*v1.0x48.0:
    1) Add API to get and set AWB measure window
*v1.0x49.0:
    1) Fix the bug that illunation name "A" is parsed incorrectly
*v1.0x50.0:
    1) Add API to set and get color temperature
*v1.0x51.0:
    1) pid which interacts with kernel replace with self-defined ID.
    2) add FilterLevelRegConf node parse in IQ files(modify by ouyang).
    3) correct version num.
*v1.0x52.0:
    1) switch TX1/RX1 D-PHY from init TX status to RX status
       before sensor stream on to avoid confliction.
*v1.0x53.0:
    1) support dual camera preview(isp0+isp1 for 3399).
*v1.0x54.0:
    1) bug exists in function HalSensorPwrSeq,fix it.
*v1.0x55.0:
    1) enable some print of getPreferedSensorRes.
    1) add getPreferedSensorResForDumpRaw interface for dump raw data to get the min framerate res.
*v1.0x56.0:
    1) commit v1.0x51.0 is not complete, so complete it.
*v1.0x57.0:
    1) state judgement in AdpfReConfigure is not right, correct it.
*v1.0x58.0:
    1) support 3840_2160P7\3840_2160P10\3840_2160P15\3840_2160P20\3840_2160P25\3840_2160P30.
*v2.1.0:
    1)support 1920_540P60\720_480P60\720_576P50\720_240P60\720_288P50.
    2)support even and odd field merge.    
*v2.0x02.0:
    1) add awbExpprior outdoor swithoff para in IQ xml. Default: switch on;
*2.0x3.0:
    1) v1.0x53.0 eixts bug, fix it.
*2.0x4.0:
    1) support rkispv12,AE&HIST merged only.
*2.0x5.0:
    1) calibdb: donot return false when meet unknown tag.
*2.0x6.0:
    1) add rkispv12 rksharpen xml param.
*2.0x7.0:
    1) correct rksharpen register define.
*2.ox8.0:
    1) IE module implement rksharpen code.
*2.0x9.0:
    1) call rksharpen interfaces.
*2.0xa.0:
    1) add rkispv12 awdr xml param parse.
*2.0xb.0:
    1) add awdr implement code.
*2.0xc.0:
    1) add awdr interfaces in CamEngineItf.
*2.0xd.0:
    1) add rkispv12 filter lp xml param parse.
*2.0xe.0:
    1) filter lp implement code.
*2.0xf.0:
    1) call filter lp interfaces.
*2.0x10.0:
    1) intepolate 5x5 gridweigth numbers from 9x9.
*2.0x11.0:
    1) Add get exposure\focus statics informations and parameter values interface.
*2.0x12.0:
    1) correct gammaout curve value number in rkispv12.
*2.0x13.0:
    1) complete B/W sensor pipeline.
*2.0x14.0:
    1) When take raw picture,af will report err,fix the bug.
*2.0x15.0:
    1) Disable awb,when sensor is white/black sensor.
*2.0x16.0:
    1) hist: fix error of reading hist last register by oyyf.
*2.0x17.0:
    1) support IQ_Tool2(awb_v11 only).
*2.0x18.0:
    1) dual sensor run on the same time, AE and awb use the first one to master device.
*2.0x19.0:
    1) support color sensor output gray image.
    2) isp yuv output full range only.
*2.0x1a.0:
    1) add resolution 2096x1560.
*2.0x1b.0:
    1) fix bugs in v2.0x17.0.
*2.0x1c.0:
    1) add the callback function which calls from mi to sensor.
*2.0x1d.0:
    1) select the isp path according to the sensor mode:
        filed mode: sp.
        frame mode: mp.
    2) setup the subCtrl of camEngine according to isp path.
*2.0x1e.0:
    1) add the support that combine odd/even field into one frame in sp.
    2) fix bug in v2.0x1d.0.
*2.0x1f.0:
    For RKIQTool2.0 AWB Version:
    1) Support AWB v10 and v11.
    2) Support UVC Command of AWB V11.
*2.0x20.0:
    1) fix bugs in v2.0x1f.0.
*2.0x21.0:
    1) Get android memory(GraphicBuffer) from ISP driver.
    2) add selectResForDumpRaw interface.
*3.0.0:
    1) Upgrade version number.
*3.1.0:
    1) burst size calculation is wrong,correct it.
*/


#define CONFIG_SILICONIMAGE_LIBISP_VERSION KERNEL_VERSION(3, 0x1, 0)



class CamEngineItf;
typedef void (AfpsResChangeCb_t)(void *ctx);

typedef struct CamEngVer_s {
    unsigned int libisp_ver;
    unsigned int isi_ver;
} CamEngVer_t;

typedef struct CtxCbResChange_s {
    void *pIspAdapter;
    uint32_t res;
}CtxCbResChange_t;

class CamEngineVersionItf
{
public:
    CamEngineVersionItf(){};
    ~CamEngineVersionItf(){};
    
    bool getVersion(CamEngVer_t *ver);
};

/**
 * @brief CamEngineItf class declaration.
 */

class CamEngineItf
{
public:
    /**
     * @brief Standard constructor for the CamEngineItf object.
     */
    CamEngineItf( HalHandle_t hHal, AfpsResChangeCb_t *pcbResChange = NULL, void *ctxCbResChange = NULL, int mipiLaneNum=1);
    ~CamEngineItf();

public:
     enum State
    {
        Invalid = 0,
        Idle,
        Running,
        Paused
    };

    enum ConfigType
    {
        None = 0,
        Sensor,
        Image
    };

    enum SnapshotType
    {
        RGB         = 0,
        RAW8        = 1,
        RAW12       = 2,
        JPEG        = 3,    /**< jpeg with exif header */
        JPEG_JFIF   = 4,    /**< jpeg with hardware generated jfif header */
        DPCC        = 5
    };

private:
    CamEngineItf (const CamEngineItf& other);
    CamEngineItf& operator = (const CamEngineItf& other);
    bool sensorModeCb(const CamEngineItf *user, CamSensorMode_t *mode);
    bool sensorFiledStatCb(const CamEngineItf *user, CamSensorFiledStat_t *FiledStat);

public:
    HalHandle_t   getHalHandle() const{return m_hHal;} // zyc add
    State       state() const;
    ConfigType  configType() const;

    CamEngineModeType_t configMode() const;

    const char *softwareVersion() const;
    uint32_t    bitstreamId() const;
    uint32_t    camerIcMasterId() const;
    uint32_t    camerIcSlaveId() const;
    bool        isBitstream3D() const;
    //zyl add
    void getAwbGainInfo(float *f_RgProj, float *f_s, float *f_s_Max1, float *f_s_Max2, float *f_Bg1, float *f_Rg1, float *f_Bg2, float *f_Rg2);
    void getIlluEstInfo(float *ExpPriorIn, float *ExpPriorOut, char (*name)[20], float likehood[], float wight[], int *curIdx, int *region, int *count);
    bool getSensorXmlVersion(char (*pVersion)[64]);
    bool getInitAePoint(float *point);
    bool setAePoint(float point);
    float getAecClmTolerance() const;
    bool setAeClmTolerance(float clmtolerance);
    bool setIspBufferInfo(unsigned int bufNum, unsigned int bufSize);
    void enableSensorOTP(bool_t enable);
    //oyyf add
    void getIspVersion(unsigned int* version);
    //oyyf add
    bool getSensorIsiVersion(unsigned int* pVersion);
    //oyyf add
    bool getSensorTuningXmlVersion(char** pTuningXmlVersion);
    //oyyf adds
    bool getLibispIsiVersion(unsigned int* pVersion) ;
    //oyyf add
    bool checkVersion(rk_cam_total_info *pCamInfo);
    //zyc add
    bool getPreferedSensorRes(CamEngineBestSensorResReq_t *req);
    bool previewSetup_ex(CamEngineWindow_t dcWin,int usr_w,int usr_h,CamerIcMiDataMode_t mode,CamerIcMiDataLayout_t layout,bool_t dcEnable);
    bool selectResForDumpRaw(CamEngineBestSensorResReq_t *req);
    bool pre2capparameter(bool is_capture,rk_cam_total_info *pCamInfo);


    // open sensor driver
    bool        openSensor( rk_cam_total_info *pCamInfo, int sensorItfIdx );
    bool        openSensor( const char *pFileName, int sensorItfIdx = 0 );

    // setup sensor for a priview resolution
    bool        previewSensor();
    // open image file
    bool        openImage( const char               *pFileName,
                           PicBufMetaData_t         image,
                           CamEngineWbGains_t *wbGains = NULL,
                           CamEngineCcMatrix_t *ccMatrix = NULL,
                           CamEngineCcOffset_t *ccOffset = NULL,
                           CamEngineBlackLevel_t *blvl = NULL,
                           float gain = 1.0f, float itime = 0.01f );

    bool        previewSetup();

    bool        hasImage();
    bool        hasSensor();

    const char *sensorDriverFile() const;
    void        clearSensorDriverFile() const;

    bool        loadCalibrationData( camsys_load_sensor_info *pLoadInfo );
    bool        loadCalibrationData( const char *pFileName );
    const char *calibrationDataFile() const;
    CamCalibDbHandle_t calibrationDbHandle() const;

    const char *sensorName() const;
    uint32_t    sensorRevision() const;
    bool        isSensorConnected() const;
    uint32_t    subSensorRevision() const;
    bool        isSubSensorConnected() const;
    const IsiRegDescription_t*
                sensorRegisterTable() const;
    bool        getSensorCaps( IsiSensorCaps_t &sensorCaps ) const;
    void        getSensorConfig( IsiSensorConfig_t &sensorConfig ) const;
    void        setSensorConfig( const IsiSensorConfig_t &sensorConfig );
    bool        getIlluminationProfiles( std::vector<CamIlluProfile_t *> &profiles ) const;
    bool        dumpSensorRegister( bool main_notsub, const char* pFileName ) const;
    bool        getSensorRegisterDescription( uint32_t addr, IsiRegDescription_t &description ) const;
    bool        readSensorRegister( bool main_notsub, uint32_t addr, uint32_t &value ) const;
    bool        writeSensorRegister( bool main_notsub, uint32_t addr, uint32_t value ) const;

    void        getScenarioMode( CamEngineModeType_t &mode ) const;
    void        setScenarioMode( CamEngineModeType_t &mode ) const;
    int         getSensorItf() const;
    void        setSensorItf( int &idx ) const;
    bool        changeSensorItf( bool restoreState = false );

    bool        is3DEnabled() const;
    void        enableTestpattern( bool enable = true );
    bool        isTestpatternEnabled() const;
    void        enableAfps( bool enable = true );
    bool        isAfpsEnabled() const;

    bool        getFlickerPeriod( float &flickerPeriod ) const;
    void        getFlickerPeriod( CamEngineFlickerPeriod_t &flickerPeriod ) const;
    void        setFlickerPeriod( CamEngineFlickerPeriod_t &flickerPeriod ) const;

    bool        registerBufferCb( CamEngineBufferCb_t fpCallback, void* BufferCbCtx );
    bool        deRegisterBufferCb();
    bool        getBufferCb( CamEngineBufferCb_t* fpCallback, void** BufferCbCtx );

    bool        mapBuffer( const MediaBuffer_t *pSrcBuffer, PicBufMetaData_t *pDstBuffer );
    bool        unmapBuffer( PicBufMetaData_t* pBuffer );

    bool reset();

    bool connect();
    void disconnect();
    bool changeResolution( uint32_t resolution, bool restoreState = false );
    bool changeEcm( bool restoreState = false );

    bool start( uint32_t frames = 0 );
    bool pause();
    bool stop();

    bool getResolution( int &width, int &height ) const;
    bool getResolution( uint32_t &resolution ) const;

    bool getGain( float &gain ) const;
    bool getGainLimits( float &minGain, float &maxGain, float &step ) const;
    bool setGain( float newGain, float &setGain );

    bool getIntegrationTime( float &time ) const;
    bool getIntegrationTimeLimits( float &minTime, float &maxTime, float &step ) const;
    bool setIntegrationTime( float newTime, float &setTime );

    bool setExposure( float newExp, float &setExp );

    bool getFocus( uint32_t &focus ) const;
    bool getFocusLimits( uint32_t &minFocus, uint32_t &maxFocus ) const;
    bool setFocus( uint32_t focus );

    bool isEvenField( IsiSensorFrameInfo_t &SensorInfo, bool &isEvenField );
    bool getDeviceInfo(const CamEngineItf *user, CamDeviceContext_t *DevInfo);
    static bool getDeviceInfoCb(void *user, CamDeviceContext_t *DevInfo);

    bool sensorModeCb(CamSensorMode_t *mode);
    static bool getSensorMode(void *user, CamSensorMode_t *mode);
    static bool getSensorFiledStat(void *user, CamSensorFiledStat_t *FiledStat);

    bool getPathConfig( const CamEngineChainIdx_t idx, CamEnginePathType_t path, CamEnginePathConfig_t &pathConfig ) const;
    bool setPathConfig( const CamEngineChainIdx_t idx, const CamEnginePathConfig_t &mpConfig, const CamEnginePathConfig_t &spConfig );

    bool searchAndLock( CamEngineLockType_t locks );
    bool unlock( CamEngineLockType_t locks );

    // auto exposure functions
    bool isAecEnabled();
    bool startAec();
    bool stopAec();
    bool resetAec();
    bool lock3a( CamEngine3aLock_t lock );  /* ddl@rock-chips.com: v0.0x29.0 */
    bool unlock3a( CamEngine3aLock_t unlock );
    bool configureAec( const CamEngineAecSemMode_t &mode, const float setPoint, const float clmTolerance, const float dampOver, const float dampUnder );
    bool getAecStatus( bool &enabled, CamEngineAecSemMode_t &mode, float &setPoint, float &clmTolerance, float &dampOver, float &dampUnder );
    bool getAecHistogram( CamEngineAecHistBins_t &histogram ) const;
    bool getAecLuminance( CamEngineAecMeanLuma_t &luma ) const;
    bool getAecObjectRegion( CamEngineAecMeanLuma_t &objectRegion ) const;
    bool getAecMeasureWindow( CamEngineWindow_t *measureWin, CamEngineWindow_t *grid );
    bool setAecHistMeasureWinAndMode( int16_t x,int16_t y,uint16_t width,uint16_t height,CamEngineAecHistMeasureMode_t mode );

    // auto focus functions
    bool isAfAvailable( bool &available );
    bool resetAf( const CamEngineAfSearchAlgorithm_t &searchAlgorithm );
    bool startAfContinous( const CamEngineAfSearchAlgorithm_t &searchAlgorithm );
    bool startAfOneShot( const CamEngineAfSearchAlgorithm_t &searchAlgorithm );
    bool stopAf();
    bool getAfStatus( bool &enabled, CamEngineAfSearchAlgorithm_t &seachAlgorithm, float *sharpness );
    bool checkAfShot( bool *shot );  /* ddl@rock-chips.com */
    bool registerAfEvtQue( CamEngineAfEvtQue_t *evtQue );   /* ddl@rock-chips.com */
    bool getAfMeasureWindow( CamEngineWindow_t *measureWin );
    bool setAfMeasureWindow( int16_t x,int16_t y,uint16_t width,uint16_t height );

    // flash   ddl@rock-chips.com
    bool configureFlash( CamEngineFlashCfg_t *cfgFsh );
    bool startFlash ( bool operate_now );
    bool stopFlash ( bool operate_now );
    
    // auto white balance functions
    bool isAwbEnabled();
    bool startAwb( const CamEngineAwbMode_t &mode, const uint32_t idx, const bool_t damp );
    bool stopAwb();
    bool resetAwb();
    bool getAwbStatus( bool &enabled, CamEngineAwbMode_t &mode, uint32_t &idx, CamEngineAwbRgProj_t &RgProj, bool &damping );
    bool setAwbForceIllumination(bool_t forceIlluFlag, char *illName);
    bool setAwbForceWbGain(bool_t forceWbGainFlag, float fRGain, float fGrGain, float fGbGain, float fBGain);
    bool getAwbForceIllumination(bool_t *forceIlluFlag, char *illName);
    bool getAwbForceWbGain(bool_t *forceWbGainFlag, float *fRGain, float *fGrGain, float *fGbGain, float *fBGain);
    bool getAwbWhitePoint(uint8_t *awb_mode, CamAwbWpGet_t *pAwbWpGet);
    bool setAwbWhitePoint(int8_t measMode, CamCalibAwb_V11_Global_t *pAwbWpSet, CamAwbMeasResult_t *pAwbmeas);
    bool getAwbCurve(CamAwbCurve_t *pAwbCurve);
    bool setAwbCurve(CamAwbCurve_t *pAwbCurve);
    bool setAwbRefWbGain(float fRGain, float fGrGain, float fGbGain, float fBGain, char *illName);
    bool getAwbRefWbGain(float *fRGain, float *fGrGain, float *fGbGain, float *fBGain, char *illName);

    bool setLscProfile(CamLscProfile_t *pLscProfile);
    bool getLscInfo(int8_t *aCCDnName, int8_t *aCCUpName, CamLscMatrix_t *lscMatrix);
    bool getLscProfile(CamLscProfile_t *pLscProfile);
    bool getCcmInfo(int8_t *aCCDnName, int8_t *aCCUpName, Cam3x3FloatMatrix_t *ccMatrix, Cam1x3FloatMatrix_t *ccOffset);
    bool setCcmInfo(int8_t *name, Cam3x3FloatMatrix_t *ccMatrix, Cam1x3FloatMatrix_t *ccOffset);

    bool chkAwbIllumination( CamIlluminationName_t   name );
    bool isAwbStable();
    bool SetAwbMeasuringWindow(uint16_t x, uint16_t y,uint16_t width,uint16_t height);
    bool getAwbMeasureWindow(uint16_t *x, uint16_t *y, uint16_t *width, uint16_t *height);
    bool getAwbTemperature(float *ct);
    bool startAdpf();
    bool getMfdGain(    char *mfd_enable,float mfd_gain[],float mfd_frames[]);    
    bool getUvnrPara( char *uvnr_enable,float uvnr_gain[],float uvnr_ratio[],float uvnr_distances[]);
    bool stopAdpf();
    bool isAdpfEnabled();
    bool configureAdpf( const float gradient, const float offset, const float min, const float div, const uint8_t sigmaGreen, const uint8_t sigmaRedBlue, uint8_t segmentation,uint16_t *nllcoeff,float *nlgains );
    bool getAdpfStatus( bool &enabled, float gradient, float offset, float min, float div, uint16_t sigmaGreen, uint16_t sigmaRedBlue, uint8_t segmentation,uint16_t *nllcoeff,float *nlgains);
    bool setDpfProfile(CamDpfProfile_t *pDpfProfile);
    bool getDpfProfile(CamDpfProfile_t *pDpfProfile);
    bool setFltParams(CamFltLevelRegConf_t *fltLevelRegConf);
    #if defined(RK_ISP_V12)
    bool startAwdr();
    bool stopAwdr();
    bool configureAwdr( const uint8_t maxGain );
    bool getAwdrStatus(bool &Running, bool &wdrEnabled, bool &MaxGainEnabled,uint8_t Wdrmode, uint8_t &maxGain);
    #endif
    bool startAdpcc();
    bool stopAdpcc();
    bool getAdpccStatus( bool &enabled );

    bool startAvs();
    bool stopAvs();
    bool configureAvs( const bool useParams, const unsigned short numItpPoints, const float theta, const float baseGain, const float fallOff, const float acceleration );
    bool getAvsConfig( bool &usingParams, unsigned short &numItpPoints, float &theta, float &baseGain, 
                       float &fallOff, float &acceleration, int &numDampData, double **ppDampXData, double **ppDampYData );
    bool getAvsStatus( bool &running, CamEngineVector_t &displVec, CamEngineVector_t &offsVec );

    bool enableJpe( int width, int height );
    bool disableJpe();

    // memory infterface module
    bool isPictureOrientationAllowed( CamEngineMiOrientation_t orientation );
    bool setPictureOrientation( CamEngineMiOrientation_t orientation );
    //bool getPictureOrientation( CamerIcMiOrientation_t &orientation );

    // image effects module 
    void imgEffectsEnable( CamerIcIeConfig_t *pCamerIcIeConfig );
    void imgEffectsDisable();
    
    void imgEffectsSetTintCb( uint8_t tint );
    void imgEffectsSetTintCr( uint8_t tint );
    void imgEffectsSetColorSelection( CamerIcIeColorSelection_t color, uint8_t threshold );
    void imgEffectsSetSharpen( uint8_t factor, uint8_t threshold );

    // color processing module 
    bool cProcIsAvailable();
    void cProcEnable( CamEngineCprocConfig_t *config );
    void cProcDisable();
    void cProcStatus( bool& running, CamEngineCprocConfig_t& config );

    void cProcSetContrast( float contrast );
    void cProcSetBrightness( float brightness );
    void cProcSetSaturation( float saturation );
    void cProcSetHue( float hue );

    // super impose module 
    void sImpEnable( CamEngineSimpConfig_t *config );
    void sImpDisable();

    // black level substraction module
    void blsGet(uint16_t *Red, uint16_t *GreenR, uint16_t *GreenB, uint16_t *Blue);
    void blsSet(const uint16_t Red, const uint16_t GreenR, const uint16_t GreenB, const uint16_t Blue);
    void blsEnable();
    void blsDisable();
    void blsIsEnabled(bool_t *pIsEnabled);
    void blsSetSubstractionMode(const uint16_t submode);
    void blsGetSubstractionMode(uint16_t *submode);
    void blsSetMeasuringWindow(const uint16_t WdwId, const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height);
    void blsGetMeasuringWindow(const uint16_t WdwId, uint16_t *x, uint16_t *y, uint16_t *width, uint16_t *height);
    void blsEnableMeasuringWindow(const uint16_t WdwId);
    void blsDisableMeasuringWindow(const uint16_t WdwId);
    bool_t blsMeasuringWindowIsEnabled(const uint16_t WdwId);
    void blsSetSamples(const uint8_t samples);
    void blsGetSamples(uint8_t *samples);

    // white balance module
    void wbGainGet(float *Red, float *GreenR, float *GreenB, float *Blue);
    void wbGainSet(const float Red, const float GreenR, const float GreenB, const float Blue);

    void wbCcMatrixGet( float *Red1, float *Red2, float *Red3,
                        float *Green1, float *Green2, float *Green3,
                        float *Blue1, float *Blue2, float *Blue3 );

    void wbCcMatrixSet( const float Red1, const float Red2, const float Red3,
                        const float Green1, const float Green2, const float Green3,
                        const float Blue1, const float Blue2, const float Blue3 );

    void wbCcOffsetGet( int16_t *Red, int16_t *Green, int16_t *Blue );
    void wbCcOffsetSet( const int16_t Red, const int16_t Green, const int16_t Blue );

    // lense shade correction module
    void lscStatus( bool& running, CamEngineLscConfig_t& config );
    void lscEnable();
    void lscDisable();
    void isLscEnabled(bool &enable);

    // chromatic abberation correction module
    void cacStatus( bool& running, CamEngineCacConfig_t& config );
    void cacEnable();
    void cacDisable();

    // wide dynamic range mode
    void wdrEnable();
    void wdrDisable();
    void wdrSetCurve( CamEngineWdrCurve_t WdrCurve );

    // gamma out correction module
    void gamCorrectStatus( bool& running, CamEngineGammaOutCurve_t *gammaCurve );
    void gamCorrectEnable();
    void gamCorrectDisable();
    void gamCorrectSetCurve();
    void gamSetCurveForTool(CamGoc_t *pCurv);
    void gamGetCurveForTool(bool& running, CamGoc_t *pCurv);


    void ColorConversionSetRange(CamerIcColorConversionRange_t YConvRange,CamerIcColorConversionRange_t CrConvRange);
    void ColorConversionSetCoefficients(CamerIc3x3Matrix_t    *pCConvCoefficients);

    // isp ilter
    void demosaicGet( bool& bypass, uint8_t& threshold ) const;
    void demosaicSet( bool bypass, uint8_t threshold );
    void filterStatus( bool& running, uint8_t& denoiseLevel, uint8_t& sharpenLevel );
    void filterEnable();
    void filterDisable();
    void filterSetLevel( uint8_t denoiseLevel, uint8_t sharpenLevel );

    // color noise reduction
    bool cnrIsAvailable();
    void cnrStatus( bool& running, uint32_t& TC1, uint32_t& TC2 );
    void cnrEnable();
    void cnrDisable();
    void cnrSetThresholds( uint32_t TC1, uint32_t TC2 );
    
    // memory interface 
    void miSwapUV( bool swap );
    
    // dual cropping unit 
    bool dcropIsAvailable();
    void dcropMpWindowGet( bool& enabled, uint16_t& x, uint16_t& y, uint16_t& width, uint16_t& height );
    void dcropMpWindowSet( bool  enable , uint16_t  x,  uint16_t y, uint16_t  width, uint16_t  height );
    void dcropSpWindowGet( bool& enabled, uint16_t& x, uint16_t& y, uint16_t& width, uint16_t& height );
    void dcropSpWindowSet( bool  enable , uint16_t  x,  uint16_t y, uint16_t  width, uint16_t  height );


    bool isSOCSensor(); // zyc add
    uint32_t getYCSequence(); //zyc add
    uint32_t getBusWidth(); //zyc add
    uint32_t reSetMainPathWhileStreaming
                (
                    CamEngineWindow_t* pWin,
                    uint32_t outWidth, 
                    uint32_t outHeight
                    
                );
    float getAecMeanLuminance() const;

private:
    void closeSensor();
    void closeImage();

    bool setupSensor();
    void doneSensor();
    void startSensor();
    void stopSensor();

    bool setupCamerIC();
    void doneCamerIC();
    bool restartMipiDrv(); //zyc add
    void setupOTPInfo();

public:
    class SensorHolder;
    class CamEngineHolder;
private:
    HalHandle_t         m_hHal;
    AfpsResChangeCb_t   *m_pcbItfAfpsResChange;
    CamSensorCb_t        m_sensorCb;

    void                *m_ctxCbResChange;
    CamSensorCb2_t       m_sensorCb2;

    SensorHolder        *m_pSensor;

    CamEngineHolder     *m_pCamEngine;
    int    mSensorBusWidthBoardSet;
};


/* @} module_name_api*/

#endif /*__CAM_ENGINE_ITF_H__*/
