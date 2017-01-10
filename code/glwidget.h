#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QImage>
#include <QList>
#include <QDir>
#include <QMutex>

#include "leapmotion/LeapListener.h"

#include "glview.h"
#include "tracking_defines.h"

#define NB_TEXTURE 8
#define BOX_SIZE 8.0f //the grid is always inside the box

using namespace Leap;

class GlWidget : public Glview
{
    Q_OBJECT
public:
    //typedef for textures management
    typedef enum {CRATE,
                  METAL,
                  FOLDER,
                  MUSIC,
                  PICTURE,
                  TEXT,
                  VIDEO,
                  BIN,
                  NONE = -1} texId_t;

    typedef enum {COPY,
                  CUT,
                  PASTE,
                  NEW_FILE,
                  NEW_FOLDER,
                  DELETE,
                  IDLE} FileAction_t;

    //simple way of describing a cube/item
    struct item_t {
        float x_;
        float y_;
        float z_;
        float size_;
        float sizeOffset_;
        float yOffset_;
        texId_t texture_;
        bool selected_;
        bool attracted_;
        bool drawn_;
        QString fileName_;
        Leap::Vector anchor_;
        //constructor
        item_t(const QString& pName = "", float pSize = 1.0f, texId_t pText = CRATE);
    };
    typedef QSet<int> GrabList_t;

private:
    LeapListener leapListener_;
    Leap::Controller controller_;

    GLuint texture_[NB_TEXTURE];

    //head positions in cm relative to screen center.
    head_t head_;
    Leap::Vector palmPos_;
    QList<item_t> itemList_;
    HandEvent::Selection_t selectionMode_;
    float boxSize_;
    int gridSize_;
    float spacing_;
    float zoomOffset_;
    float maxZoom_;
    bool grabbing_;
    bool block_swipe_;

    QDir fileExplorer_;

    mutable QMutex mutexList_;
    GrabList_t grabList_;
    QList<Leap::Vector> lastPos_;
    item_t bin_;
    FileAction_t currentAction_;

public:
    GlWidget(QWidget *parent = 0);
    ~GlWidget();

    //opengl functions
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void loadTexture(QString textureName, texId_t pId);
    void drawCube(texId_t PtextureId,
                  float pCenterX,
                  float pCenterY,
                  float pCenterZ,
                  float pSize);
    void drawCube(const item_t& pCube);
    void drawTile(texId_t PtextureId,
                  float pCenterX,
                  float pCenterY,
                  float pCenterZ,
                  float pSize);
    void drawTile(const item_t& pItem);

    void computeWaveGrid(int pItemPerLine = 6);
    void drawPalmPos();
    void drawCurrentGrid();
    void reloadFolder();
    void changeDirectory(const QString& pFolder);
    void displayPath();

private:
    void generateCubes(texId_t pTexture, int pNbCubes);
    void computeGrid(float pPboxSize = BOX_SIZE);
    void computeTube(int pItemPerCircle = 8);
    int closestItem(float pTreshold);
    void handleSelection();
    void handleHovering();
    void handleAttraction();
    void handleGrab();
    void customEvent(QEvent* pEvent);
    void doCopy(int pDestination);

signals:

public slots:
    void slotNewHead(head_t pPos);
    void slotMoveHead(int pAxis, float pDelta);
    void slotPalmPos(Leap::Vector pPos);

private slots:
    void slotSelect(int pItem = -1);
    void slotAttract(int pItem = -1);
    void slotTimerStopped();
    void slotDeleteSelected(void);
};

#endif // GLWIDGET_H
