#include "../baikePub/shouci.h"
#include "chaxun.h"
#include <winver.h>
#include <windows.h>
#include "../baikePub/BaikeTypeData.h"
using namespace std;

#include <QApplication>

///
/// 写点说明，时间长了就忘了。
/// 全自动，必须F2启动：在任务列表页面，鼠标放在词条上，按F2启动
/// 已经打开的，F1启动，填完以后，还得返回到F2启动，才能全自动。
/// 原理：
/// F2启动，记录鼠标的位置，以便自动移动到下一个词条上。
///

//void shifang()
//{
//    for(mvk& mv:modvk)
//    {
//        bool jg=UnregisterHotKey(NULL, mv.at);
//        if(!jg){
//            //snd<<"unreg err="<<GetLastError()<<",id="<<mv.at<<",key="<<mv.idstr;
//        }
//    }
//}
BOOL WINAPI ConsoleCtrlhandler(DWORD dwCtrlType)
{
    //控制台消息循环抓不住wm_close wm_quit wm_destroy
    //需要使用SetConsoleCtrlHandler(ConsoleCtrlhandler, TRUE);在退出前释放注册的hotkey
    //经使用发现hotkey在程序结束时自动失效，不必释放。
    //如果提供更改hotkey的功能则可以使用释放
    if(dwCtrlType == CTRL_CLOSE_EVENT)
    {
        snd<<"closeEvent";
        HWND hwndGuanbi = FindWindowW(NULL, L"关闭");
        if(hwndGuanbi!=NULL){
            DWORD tid=GetWindowThreadProcessId(hwndGuanbi,NULL);
            PostThreadMessageW(tid,WM_USER+msgCloseGbid,0,0);
        }
        //shifang();
    }
    return TRUE;
}

int main(int argc, char *argv[])
{
    UINT sjc[][2][1]={
        //半屏幕
        {
            //放弃y
            {7},
            {7}
        },
        //满屏幕
        {
            //---------------高100%
            //放弃y
            {753},
            //---------------低90%
            {682}

        }
    };
    QApplication a(argc, argv);

    Q_UNUSED(argc);
    QThreadPool::globalInstance()->setMaxThreadCount(2);

    Baseop bop;

    //int lianjieYinyue=bop.vkAdd("lianjieYinyue",VK_F5);
    //
    int vkPutong=bop.vkAdd("vkPutong",VK_F8);

    bool isRebootself=bop.init(L"书籍封面图--F6强制关闭",pzManping,true,
                               argv[0],
            QString("\r\n Edge浏览器，页面100%显示"
                    "\r\n\r\n F1提交"
                    "\r\n\r\n F2连续"
                    "\r\n\r\n F3--启动输入确定功能"
                    "\r\n\r\n F8普通概述图"
                    ),
            QString("\r\n Edge浏览器，页面90%显示"
                    "\r\n\r\n F1提交"
                    "\r\n\r\n F2连续"
                    "\r\n\r\n F3--输入确定功能"
                    "\r\n\r\n F8普通概述图"
                    )
            );
    bop.renwuZuobiao(sjc[bop.banping][bop.pingmu][0]);

    Shouci sc(&bop);
    Chaxun dydata;
    dydata.setBop(&bop);

#define tiaoshi 10
#define danzishiyan 10
#if(tiaoshi==1)
    QString sm[]={
     "国","两严","两严两","两严两严","两严两严两","两严两严两严","两严两严两严严"
    };
    QString zz[]={
         "无法触碰"
    };
#if(danzishiyan==1)
    //单字实验
    int x,y,s,j,zs;
    //WCHAR dz[];
    for(int i=0;i<100;i++){
        sow<<":";
        cin>>zs;
        cin>>x;
        cin>>y;
        cin>>s;
        cin>>j;
        //wcin>>dz;
        //sow<<QString::fromWCharArray(&dz);
        bop.jpgFile=QString("D:\\临时传图\\gst%1.jpg").arg(2,2,10,QChar('0'));
        bop.zidongHuatuTianzi("两",x,y,s,j);
    }
#else
    const int len=7;
    for(int i=0;i<len;i++){
        bop.jpgFile=QString("D:\\临时传图\\gst%1.jpg").arg(i,2,10,QChar('0'));
        bop.zidongHuatuTianzi(sm[i]);
    }
#endif
#endif
    //
    if(!isRebootself){
        sow<<QString("自动、按键F3表示：");
        sow<<QString("0 歌曲图");
        sow<<QString("1 竖版书籍图");
        sow<<QString("2 普通图");
        sow<<QString("3 田字格图");
        sow<<QString("请输入：");
        cin>>bop.xuanze1;
        //tijiaoFenlei
        sow<<QString("提交时勾选：");
        sow<<QString("0 更正错误");//tfGengzhengCuowu
        sow<<QString("1 修改图片");//tfXiugaiTupian
        sow<<QString("请输入：");
        cin>>bop.xuanze2;
    }
    //
    SetConsoleCtrlHandler(ConsoleCtrlhandler, TRUE);
    MSG msg;
    PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        switch(msg.message)
        {
        //case WM_DESTROYCLIPBOARD://控制台窗口收不到这个消息
        //    sow<<"delclip";
        //    break;
        case WM_USER+msgHotkey:
            bop.zhucerejian();
            if(isRebootself){
                isRebootself=false;
                bool jg=bop.xiayige();
                if(jg){
                    sc.runscript();
                }else{
                    sow<<"chognqi error";
                }
            }
            break;
        case WM_HOTKEY:
        {
            UINT ctrlalt=LOWORD(msg.lParam);
            UINT anjian=HIWORD(msg.lParam);
            if(ajjc(vkiBtnleft)){
                bop.LeftClick();
            }else if(ajjc(vkiTijiao)){
                switch(bop.xuanze1){
                case 1:
                    dydata.tijiaoBefore();//竖版书籍封面图片调整完成后，F1点击两个确定
                    bop.tijiao(tijiaoFenlei(bop.xuanze2),true);
                    break;
                default:bop.tijiao(tijiaoFenlei(bop.xuanze2),true);break;
                }
            }else if(ajjc(vkiFangqi)){
                sow<<"an jian fang qi";
                bop.fangqi();
            }else if(ajjc(vkiLianxu)){
                //记录鼠标位置并打开词条
                sc.runscript();
                //开始填表
            }else if(ajjc(vkiTianbiao)){
                switch(bop.xuanze1){
                case 0:dydata.yinyue();break;
                case 1:dydata.shujifengmian(false);break;
                case 2:dydata.putong();break;
                case 3:dydata.tianzige();break;
                }
                //--------------------F12346已注册---------------------------------------------
            }
            else if(ajjc(vkPutong)){//ajjc按键检测
                dydata.putong();
                //dydata.shujifengmian(true);//横版书籍封面
            }
            //else if(ajjc(lianjieYinyue)){
            //    dydata.lianjieYinyue();
            //}
        }
            break;
        default:
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return a.exec();
}
