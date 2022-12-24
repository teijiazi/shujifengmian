#include "chaxun.h"
#include <iostream>
#include <QFile>
#include <urlmon.h>
#include <QImage>
#include <QPainter>
#include <QThreadPool>
#include "../rizhi/sslhttp.h"

void Chaxun::tijiaoBefore(){
    //图片不能自动调整。手工调整后，F1点击确定
    UINT sjc[][2][20]={
        //屏幕是75% 占据左半边
        {
            //添加概述图 编辑概述图册2,3  本地45     等待"打开"出来67  本地图加载完89   "封面"按钮10 11
            //         上传图片边的问号                            侧边灰色背景
            {600,433,  445,231,       560,232,   593,513,       516,329,       407,409,
             //  编辑图册12 13   编辑图册结束14 15        确定16 17  结束18 19
             //  弹出页面的左边沿  显示裁剪框，坐标是深色蒙层
             372,478,       911,347,               912,138,  952,400},
            {851,507,   682,259,      835,258,   323,451,       619,356,       70,147,
             41,327,        938,85,                35,231}
        },

        //占满
        {
            //添加概述图 编辑概述图册2,3  本地45     "打开"底边背景67  本地图加载完89   "封面"按钮10 11
            //         上传图片边的问号                            预览图上侧边灰色背景【横版图】
            //-------------------------高屏幕 100%,------------------------------------------------------
            {1236,533,  1229,258,     1386,260, 439,422,        1322,345,       1182,498,
             //编辑图册12 13   编辑图册结束14 15        确定16 17  结束18 19
             //弹出页面的左边沿  -显示裁剪框，坐标是      页面侧边
             //               -无图处深色蒙层
             //               -mubanCiyu
             1152,395,        1867,470,             1871,135,  1863,363},

            //-------------------------低屏幕 90%,--------------------------------------------------------
            {931,490,   750,245,      887,245, 350,397,        810,321,       701,460,
             659,422,        1327,390,              1328,128,  1357,345}
        }

    };
    //while(!yanse(12,13,0xF8F8F8)){//等待"编辑图册"页面出来 编辑图册页面左边沿，前一页时同位置是ffffff
    //    Sleep(100);
    //}
    //Sleep(500);//等动画结束
    //while(!yanse(14,15,0x313131)){//等待"编辑图册"页面展示完毕 裁剪框显示出来--右侧深色蒙层 mubanCiyu mubanShujiShu
    //    Sleep(100);
    //}
    dian(16,17);//确定
    while(!yanse(14,15,0xFFFFFF)){//等待裁剪结束，返回到“编辑概述图册”
        Sleep(100);
    }
    Sleep(500);//等动画结束
    dian(16,17);//确定
    //等动画结束，图片编辑完全退出
    while(!yanse(18,19,0xF8F8F8)){
        Sleep(100);
    }
    Sleep(200);
}

void Chaxun::setBop(Baseop *vbop)
{
    bop=vbop;
}

void Chaxun::reset(){
    xuyaoShuminghao=false;
    duoyuShuminghao=false;
    xuyaoJuhao=false;
    xinxilanXuyaoZuozhe=false;
    xinxilanXuyaoPingtai=false;
    xinxilanXuyaoLeixing=false;
    xinxilanXuyaoZhuangtai=false;
    xinxilanXuyaoZhongwenming=false;
    pingtaiShiBaidu=false;
}

bool Chaxun::xiadaobendi(const QString& tpurl, const QString &wjm){
    QString jpgfile(wjm);
    HRESULT hrjg = URLDownloadToFileW(NULL,
                                      tpurl.toStdWString().c_str(),//如果图片是PNG，下载后存储为1905.jpg，
                                      jpgfile.toStdWString().c_str(),//图片内容不变，格式仍然是png
                                      0, NULL);
    if (hrjg != S_OK)
    {
        sow <<"downloaderr=" <<hrjg;
        return false;
    }
    return true;
}

bool Chaxun::fangda(const QString& yuanshiJpg,const QString& fangdahouJpg)
{
    QImage x;
    int w=0,h=0;
    if(x.load(yuanshiJpg)){//读取1905.jpg，不填格式，自动识别实际格式
        int minsize=120;
        w=x.width();
        h=x.height();
        if(w<minsize && h<minsize){
            snd<<"tutaixiao,w="<<w<<",h="<<h;
            return false;
        }
    }else{
        snd<<"loadjpg fail";
        return false;
    }

    int maxsize=800;
    QImage xx;
    if(w==h && w<maxsize){
        xx=x.scaledToWidth(maxsize,Qt::SmoothTransformation);
    }
    else if(w<h && w<maxsize)
    {
        xx=x.scaledToWidth(maxsize,Qt::SmoothTransformation);
    }else if(h<w && h<maxsize){
        xx=x.scaledToHeight(maxsize,Qt::SmoothTransformation);
    }else{
        xx=x;
    }
    return xx.save(fangdahouJpg);//如果是PNG格式，save会自动转换为jpg格式。
}

void Chaxun::yasuoLiuchicun(const QString& yuanshiTupian, const QString & mubiao){
    //保留尺寸压缩字节
    QImage x;
    if(x.load(yuanshiTupian)){//读取1905.jpg，不填格式，自动识别实际格式
        int w=x.width();
        int h=x.height();
        int maxsize=800;
        QImage xx;
        if(w==h && w!=maxsize){
            xx=x.scaledToWidth(maxsize,Qt::SmoothTransformation);
        }else if(w<h && w!=maxsize){
            xx=x.scaledToWidth(maxsize,Qt::SmoothTransformation);
        }else if(h<w && h!=maxsize){
            xx=x.scaledToHeight(maxsize,Qt::SmoothTransformation);
        }else{
            xx=x;
        }
        xx.save(mubiao,"jpg",70);
    }else{
        snd<<"loadjpg fail";
    }
}
void Chaxun::lianjieYinyue(){
    QString tpurl=bop->GetCLB();
    tianjiaUrlGaishutu(tpurl);
}
void Chaxun::yinyue(){
    //人工观测，是歌曲，且需要图片.从音乐网站获取歌曲的图片链接并添加概述图
    QString qw;
    bool youGaishutu=false;
    YuanmaFenxi yf=bop->GetInfoFormEdit(qw);
    citiaoMingcheng=yf.citiaoMing;
    youGaishutu=yf.youGaishutu;

    if(citiaoMingcheng.contains(QRegularExpression(bop->btwj)) || youGaishutu){
        //已有图，需要放弃
        Sleep(1500);
        bop->fangqi();
        return;
    }
    if(qw.contains(QRegularExpression(bop->nrwj))){
        //已有图，需要放弃
        Sleep(1500);
        bop->fangqi();
        return;
    }


    //获取参考资料
    QRegularExpression regD("id=\"J\\-ref\\-.*?href=\"(https?\\://.*?)\".*?</a>.*?>\\[引用日期");
    auto match = regD.match(qw);
    zlurl="";
    if (match.hasMatch()) {//dddddpppp
        zlurl = match.captured(1);
    }

    //snd<<zlurl;
    //分析参考资料---------------地址预处理
    QString web;
    QString tpurl;
    if(zlurl.indexOf("i.y.qq.com")!=-1){
        //链接   https://i.y.qq.com/v8/playsong.html?songid=278993035&source=yqq#wechat_redirect
        //转换后  https://y.qq.com/n/ryqq/songDetail/278993035
        QRegularExpression reg("songid=(.*?)&");
        match = reg.match(zlurl);
        if (match.hasMatch()) {
            zlurl=QString("https://y.qq.com/n/ryqq/songDetail/%1").arg(match.captured(1));
        }
    }else if(zlurl.indexOf("y.qq.com/n/yqq")!=-1){
        //链接   https://y.qq.com/n/yqq/song/002FzSeb17MsSV.html
        //转换后 https://y.qq.com/n/ryqq/songDetail/002FzSeb17MsSV
        QRegularExpression reg("n/yqq/song/(.*?)\\.");
        match = reg.match(zlurl);
        if (match.hasMatch()) {
            zlurl=QString("https://y.qq.com/n/ryqq/songDetail/%1").arg(match.captured(1));
            snd<<zlurl;
        }
    }
    //snd<<zlurl;

    //------------------------内容分析--------------------------------------------------------------------------------------------
    if(zlurl.indexOf("y.qq.com")!=-1){
        qqTupian(tpurl);
    }else if(zlurl.indexOf("music.163.com")!=-1){
        wangyiTupian(tpurl);
    }else if(zlurl.indexOf("music.migu.cn")!=-1){
        miguTupian(tpurl);//先下载到本地
    }else if(zlurl.indexOf("kugou.com/song")!=-1){
        kugouTupian(tpurl);
    }else if(zlurl.indexOf("kuwo.cn")!=-1){
        kuwoTupian(tpurl);//先下载到本地
    }else if(zlurl.indexOf("xiami.com")!=-1){
        xiamiTupian(tpurl);
    }else if(zlurl.contains(QRegularExpression("isomusic.cn"))){
        tpurl="";
    }else{
        snd<<zlurl;
        bop->showWindow();
        return;
        snd<<QString("新地址");
    }
    tianjiaUrlGaishutu(tpurl);
}

void Chaxun::xiamiTupian(QString& tpurl){
    tpurl="";
    return;
}

void Chaxun::qqTupian(QString& tpurl){
    //qq音乐推广链接
    QString web;

    if(zlurl.indexOf("c.y.qq.com")!=-1){
        //https://c.y.qq.com/base/fcgi-bin/u?__=tgF3X9B
        SslHttp sh(web);
        sh.getData(zlurl);
        //snd<<web;
        //<img class="qui_list__img" src="https://y.gtimg.cn/music/photo_new/T001R150x150M000002wbJWA0kD8Tx_1.jpg?max_age=2592000"/></div>
        //                                https://y.gtimg.cn/music/photo_new/T001R800x800M000002wbJWA0kD8Tx_1.jpg
        //参考                                https://y.qq.com/music/photo_new/T001R300x300M000002wbJWA0kD8Tx.jpg?max_age=2592000
        QRegularExpression reg("qui_list__img\" src=\"(.*?)\\?");
        auto match = reg.match(web);
        if (match.hasMatch()) {//dddddpppp
            tpurl = match.captured(1).replace("R150x150","R800x800");
        }
        if(tpurl.indexOf("YEFM2qGf9q")!=-1){//qq音乐的默认图片
            tpurl="";
        }
    }else if(zlurl.indexOf("y.qq.com")!=-1){
        //qq音乐
        //https://y.qq.com/n/ryqq/songDetail/003YW17R1YwJqZ
        SslHttp sh(web);
        sh.getData(zlurl);
        //snd<<web;
        //"coverUrl":"\u002F\u002Fy.qq.com\u002Fmediastyle\u002Fglobal\u002Fimg\u002Falbum_300.png?max_age=2592000"}],"notFound":0
        //"coverUrl":"\u002F\u002Fy.qq.com\u002Fmusic\u002Fphoto_new\u002FT002R300x300M000000YzIaq12MN8m_1.jpg?max_age=2592000"
        //补https https://y.qq.com/music/photo_new/T002R300x300M000000YzIaq12MN8m_1.jpg?max_age=2592000
        //补https https://y.qq.com/music/photo_new/T002M000000YzIaq12MN8m_1.jpg
        QRegularExpression reg("您查看的歌曲已下架");
        auto match = reg.match(web);
        if (match.hasMatch()) {
            tpurl = "";
        }else{
            reg.setPattern("coverUrl\"\\:\"(.*?\\.jpg)\\?");
            match = reg.match(web);
            if (match.hasMatch()) {
                tpurl = "https:"+match.captured(1).replace("\\u002F","/").replace("R300x300","R800x800");
            }
        }
        if(tpurl.indexOf("YEFM2qGf9q")!=-1){//qq音乐的默认图片
            tpurl="";
        }
    }
}

void Chaxun::wangyiTupian(QString& tpurl){
    //win10 ua
    //'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.5060.134 Safari/537.36 Edg/103.0.1264.77'
    zlurl.replace("#","m").replace("music.163.com","y.music.163.com");
    //snd<<zlurl;
    //网易音乐
    //https://music.163.com/#/song?id=27721247
    //https://y.music.163.com/m/song?id=27721247
    QString web;
    SslHttp sh(web);
    sh.getData(zlurl);
    //snd<<web;
    //两种模式
    //class="j-img" data-src="http://p1.music.126.net/x5L5UOT2-yl36Oi78TC6JQ==/2544269907957580.jpg">
    QRegularExpression reg("class=\"j\\-img\" data\\-src=\"(.*?)\"");
    auto match = reg.match(web);
    if (match.hasMatch()) {//dddddpppp
        tpurl = match.captured(1);
    }

    //另一种网页
    if(tpurl==""){
        //程序获取到的内容
        //<meta property="og:image" content="http://p1.music.126.net/lH5ZfbG0bjl8JXbamMMyPA==/109951166152788765.jpg" />
        //<meta property="og:image" content="http://p1.music.126.net/lH5ZfbG0bjl8JXbamMMyPA==/109951166152788765.jpg" />
        //网页右键得到的链接
        //                                   http://p2.music.126.net/lH5ZfbG0bjl8JXbamMMyPA==/109951166152788765.jpg?param=130y130
        reg.setPattern("og\\:image\" content=\"(.*)\"");
        match = reg.match(web);
        if (match.hasMatch()) {//dddddpppp
            tpurl = match.captured(1);
        }
    }

    //网易提供默认图片
    if(tpurl.indexOf("music.126.net/sRUOe_3E2U2qDCK1nq2y9A==/109951163288502715.jpg")!=-1 ||
            tpurl.indexOf("music.126.net/UeTuwE7pvjBpypWLudqukA==/3132508627578625.jpg")!=-1){
        tpurl="";
    }
    //右键图片是p2
    tpurl.replace("p1.music","p2.music");
    //限制尺寸
    if(tpurl.endsWith(".jpg")){
        tpurl+="?param=800y800";
    }
}

void Chaxun::miguTupian(QString& tpurl){
    // 咪咕音乐 下载到本地
    // https://music.migu.cn/v3/music/song/6993872BKL0
    // https://music.migu.cn/v3/music/song/6005752J03B
    QString web;
    SslHttp sh(web);
    sh.getData(zlurl);
    //snd<<web;
    //data-original="//cdnmusic.migu.cn/picture/2021/1121/0828/ASff6f28efc228b7868e9ce02f6cff03d5.jpg">
    QRegularExpression reg("data-original=\"(.*?)\"");
    auto match = reg.match(web);
    if (match.hasMatch()) {
        QString smdx=match.captured(1).trimmed().replace("S","L");
        if(smdx!=""){
            tpurl = "https:"+smdx;
        }
    }
    if(tpurl!=""){
        bool jg=xiadaobendi(tpurl,bop->jpgFile);
        if(jg){
            bop->gaishutuBendi();
            //bop->tijiao(tfXiugaiTupian,true);
            return;
        }
    }
    //snd<<tpurl;
}

void Chaxun::kugouTupian(QString& tpurl){
    //酷狗音乐 新窗口打开找图
    // https://www.kugou.com/song#hash=asdfasfd&album_id=12321
    // https://t4.kugou.com/song.html?id=2dWMm3exYV2
    bop->CtrlJia('T');
    Sleep(100);
    bop->SetCLB(zlurl);
    Sleep(500);
    bop->CtrlJia('V');
    bop->KeyPress(VK_RETURN);
    Sleep(600);
    QString kgqw;
    UINT kugoucaidan[][2][2]={
        {
            {},{}
        },
        {//满
         {209,392},//高
         {210,336}//低
        }
    };
    bop->GetInfoFormEdit(kgqw,kugoucaidan[bop->banping][bop->pingmu][0],kugoucaidan[bop->banping][bop->pingmu][1]);
    //SslHttp sh(web);
    //sh.getData(zlurl);
    //snd<<kgqw;
    //snd<<zlurl;
    QRegularExpression reg("class=\"albumImg\">.*?src=\"(.*?)\"");
    auto match = reg.match(kgqw);
    if (match.hasMatch()) {
        tpurl = match.captured(1);
    }
    bop->CtrlJia('W');
    Sleep(200);
}

void Chaxun::kuwoTupian(QString& tpurl){
    //酷我音乐 下载到本地放大
    // http://bd.kuwo.cn/yinyue/4179467?from=baidu
    // bd.kuwo.cn/play_detail/4179467?from=baidu
    //
    QString web;
    SslHttp sh(web);
    sh.getData(zlurl);
    //snd<<web;
    //pic120:"https:\u002F\u002Fimg1.kuwo.cn\u002Fstar\u002Falbumcover\u002F120\u002F14\u002F85\u002F1092043997.jpg",
    QRegularExpression reg("pic120\\:\"(.*?)\"");
    auto match = reg.match(web);
    if (match.hasMatch()) {
        QString smdx=match.captured(1).trimmed().replace("\\u002F","/").replace("/120/","/800/");
        if(smdx!=""){
            tpurl = smdx;
        }
    }
    if(tpurl!=""){
        bool jg=xiadaobendi(tpurl,bop->jpgLinshi);
        if(jg){
            jg=fangda(bop->jpgLinshi,bop->jpgFile);
            if(!jg){
                snd<<"fangdashibai";
            }
            bop->gaishutuBendi();
            bop->tijiao(tfXiugaiTupian,true);
            return;
        }
    }
}

void Chaxun::tianjiaUrlGaishutu(const QString& tpurl){
    //snd<<tpurl;
    if(tpurl==""){
        bop->zidongHuatuCiyu(citiaoMingcheng);
        bop->gaishutuBendi();
        //提交
        //bop->tijiao(tfXiugaiTupian,true);
        return;
    }
    //给词条添加概述图
    bop->gaishutuUrl(tpurl);
    Sleep(200);
    //提交
    //bop->tijiao(tfXiugaiTupian,false);
}
void Chaxun::tianzige(){
    //需要田字格文字图
    QString qw;
    YuanmaFenxi yf=bop->GetInfoFormEdit(qw);
    citiaoMingcheng=yf.citiaoMing;

    //自动补图
    bop->zidongHuatuTianzi(citiaoMingcheng);
    //给词条添加概述图
    bop->gaishutuBendi();
    Sleep(200);
    //提交
    //bop->tijiao(tfXiugaiTupian,false);
}
void Chaxun::putong(){
    //人工观测，不是歌曲或找不到图，且需要图片--普通词语概述图
    QString qw;
    YuanmaFenxi yf=bop->GetInfoFormEdit(qw);
    citiaoMingcheng=yf.citiaoMing;

    //自动补图
    bop->zidongHuatuCiyu(citiaoMingcheng);
    //给词条添加概述图
    bop->gaishutuBendi();
    Sleep(200);
    //提交
    //bop->tijiao(tfXiugaiTupian,false);
}

void Chaxun::shujifengmian(bool heng)
{
    //从背景色查看是否有概述图
    if(bop->youGaishutu()){
        bop->fangqi();
        return;
    }
    //从词条编辑页面提取有用信息
    QString qw;
    YuanmaFenxi yf;
    int shiyanMax=3;
    int shiyan=0;//试验三次
    int webLength=200;//网页内容至少200字符
    while(qw.length()<webLength && shiyan<shiyanMax){
        shiyan+=1;
        qw="";
        sow<<(QString("复制源码,第%1次").arg(shiyan));
        yf=bop->GetInfoFormEdit(qw);
    }
    if(qw.length()<webLength){
        sow<<QString("获取不到网页内容，需要退出重启");
        bop->showWindow();
        return;
    }
    citiaoMingcheng=yf.citiaoMing;
    //youGaishutu=yf.youGaishutu;

    //if(youGaishutu){
    //    //已有图，需要放弃
    //    Sleep(1500);
    //    sow<<"you tu fang qi"<<qw.length();
    //    if(qw.length()<200){
    //        sow<<qw;
    //    }
    //    bop->fangqi();
    //    return;
    //}

    if(citiaoMingcheng.contains(QRegularExpression(bop->btwj))){
        //已有图，需要放弃
        Sleep(1500);
        bop->fangqi();
        return;
    }

    zz="";

    QRegularExpression regD;
    regD.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match;
    //从概述获取
    if(zz==""){
        //《青冥器》是青青小豆芽创作的网络小说，发表于起点网。
        regD.setPattern("id=\"J-lemma-abstract-context-word\".*?是([^<》]+?)(?:创作|写)的网络小说");
        match = regD.match(qw);
        if (match.hasMatch()) {
            zz=match.captured(1);
        }
    }
    if(zz==""){
        //从信息栏获取
        regD.setPattern("作者(?:<.*?>)+(.*?)(?:<.*?>)*类型");
        match = regD.match(qw);
        if (match.hasMatch()) {//dddddpppp
            zz = match.captured(1);
        }
    }

    if(zz=="" || zz.contains(QRegularExpression(bop->btwj))){
        //zz有违禁词，无法处理的词，系统审核通不过的词 没有作者
        Sleep(1500);
        bop->fangqi();
        return;
    }

    //自动补图
    if(heng){
        bop->zidongHuatu(citiaoMingcheng,zz);
    }else{
        bop->zidongHuatuShu(citiaoMingcheng,zz);
    }
    //给词条添加概述图
    bop->gaishutuBendi(tfSanbisi);
    Sleep(200);
    //提交
    //bop->tijiao(tfXiugaiTupian);
}




