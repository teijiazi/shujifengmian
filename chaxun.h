#ifndef MIGUYINYUE_H
#define MIGUYINYUE_H
#include "../rizhi/httpreq.h"
#include "../baikePub/baseop.h"
#include <QMutexLocker>

enum LianzaiZhuangtai{
    lzLianzaizhong=0,//!<连载中
    lzZanting=1,     //!<暂停更新
    lzYiwanjie=2,    //!<完结
    lzMeiyou       //!<没有找到
};

class Chaxun: public QObject
{
    Q_OBJECT
public:
    explicit Chaxun(QObject *parent = 0):QObject(parent),
        bop(nullptr),xuyaoShuminghao(false),
        duoyuShuminghao(false),
        xuyaoJuhao(false),
        xinxilanXuyaoZuozhe(false),
        xinxilanXuyaoPingtai(false),
        xinxilanXuyaoLeixing(false),
        xinxilanXuyaoZhuangtai(false),
        xinxilanXuyaoZhongwenming(false),
        pingtaiShiBaidu(false),
        hr(nullptr),
        youGaishutu(false)
    {
    }
    ~Chaxun(){}
    ///人工观测，不是歌曲，且需要图片
    void putong();
    void setBop(Baseop *vbop);
    Baseop *bop;
    //
    bool xuyaoShuminghao;
    bool duoyuShuminghao;
    bool xuyaoJuhao;
    bool xinxilanXuyaoZuozhe;
    bool xinxilanXuyaoPingtai;
    bool xinxilanXuyaoLeixing;
    bool xinxilanXuyaoZhuangtai;
    bool xinxilanXuyaoZhongwenming;
    bool pingtaiShiBaidu;
    //
    QString zlurl;//!<第一个参考资料url
    //
    void reset();
    //
    QNetworkAccessManager nam;
    void runscript();
    ///人工观测，是歌曲，且需要图片。从音乐网站获取歌曲的图片链接并添加概述图
    void yinyue();
    /// 已经得到图片url，开始添加概述图
    void tianjiaUrlGaishutu(const QString &tpurl);
    /// 图片链接已经人工复制放在剪贴板
    void lianjieYinyue();
    ///图片下载到本地,wjm 指定存储位置和名称，可使用 jpgFile jpgLinshi
    bool xiadaobendi(const QString &tpurl, const QString &wjm);
    ///放大图片。yuanshiJpg-->fangdahouJpg
    bool fangda(const QString &yuanshiJpg, const QString &fangdahouJpg);
    ///保留尺寸压缩字节 yuanshiTupian--->mubiao
    void yasuoLiuchicun(const QString &yuanshiTupian, const QString &mubiao);
    ///书籍概述图指定书排版封面，人工调整
    void shujifengmian(bool heng);
    ///图片不能自动调整。手工调整后，F1点击确定
    void tijiaoBefore();
    ///需要田字格文字图
    void tianzige();
private:
    HttpReq *hr;
    QString citiaoMingcheng;
    QString zz;
    QString pingtai;
    bool youGaishutu;
    void kuwoTupian(QString &tpurl);
    void kugouTupian(QString &tpurl);
    void miguTupian(QString &tpurl);
    void wangyiTupian(QString &tpurl);
    void qqTupian(QString &tpurl);
    void xiamiTupian(QString &tpurl);
};

#endif // MIGUYINYUE_H
