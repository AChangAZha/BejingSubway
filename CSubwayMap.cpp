#include "CSubwayMap.h"
#include "CIntersite.h"
#include "CStation.h"
#include "CLine.h"
#include "PriorityQueue.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QTextStream>
#include <QColor>
#include <QMessageBox>

#define INF 0x3f3f3f3f

//添加线路
bool CSubwayMap::AddLine(const QString &lineName, const QColor &color, const int &charges, const QString &firstTime, const QString &lastTime)
{
    if (SearchLine(lineName) != NULL)
    {
        return false;
    }
    CLine *const newLine = new CLine(lineName, color, charges, firstTime, lastTime);
    line.Add(newLine);
    newLine->index = line.GetSize() - 1;
    numOfLine++;
    return true;
}

//添加站点
CStation *CSubwayMap::AddStation(const int &lineNum,
                                 const QString &stationName,
                                 const bool &en, const bool &ex,
                                 const double &lng,
                                 const double &lat, CStation *sta)
{
    bool find = false;
    bool addLink = false;
    SeqList<CStation *> trans;
    if (sta != NULL)
    {
        addLink = true;
    }
    else
    {
        sta = SearchStation(stationName);
    }
    if (sta != NULL)
    {
        if (sta->line == lineNum)
        {
            return NULL;
        }
        trans.Add(sta);
        CIntersite *next = sta->link;
        while (next != NULL)
        {
            if (next->distance == 0)
            {
                sta = next->station;
                if (sta->line == lineNum)
                {
                    return NULL;
                }
                else
                {
                    trans.Add(sta);
                }
            }
            next = next->link;
        }
        find = true;
    }
    CStation *newSta;
    if (!find)
    {
        newSta = line[lineNum]->AddStation(stationName, en, ex, lng, lat, false);
    }
    else
    {
        if (!addLink)
        {
            QMessageBox box;
            box.setWindowTitle("添加站点");
            box.setWindowIcon(QIcon(":/icon/icon/logo.png"));
            box.setIcon(QMessageBox::Warning);
            box.setText("其他线路已存在该站点，确定要添加吗？此操作将会设置该站点为换乘站，您所填写的信息将会失效");
            box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            box.setDefaultButton(QMessageBox::Ok);
            if (box.exec() == QMessageBox::Cancel)
            {
                return NULL;
            }
        }
        newSta = line[lineNum]->AddStation(stationName, trans[0]->entrance, trans[0]->exit, trans[0]->longitude, trans[0]->latitude, true);
        for (int k = 0; k < trans.GetSize(); k++)
        {
            trans[k]->transfer = true;
            newSta->AddLink(trans[k], 0, true, mainWindow, 0, true);
        }
    }
    newSta->mapitem = mainWindow->PaintStation(newSta, mainWindow->ui->subwayMap->scene());
    numOfStation++;
    return newSta;
}

//添加站点连接
bool CSubwayMap::AddLink(CStation *const from, CStation *const to,
                         const int &dist, const bool &pass, int const &charges,
                         const bool &bothway, const bool &fromTrans)
{
    if (!from->transfer && !to->transfer)
    {
        if (from->line == to->line)
        {
            return from->AddLink(to, dist, pass, mainWindow, charges, bothway);
        }
        else if (fromTrans)
        {
            CStation *newSta = AddStation(to->line, from->name, true, true, 0, 0, from);
            return newSta->AddLink(to, dist, pass, mainWindow, charges, bothway);
        }
        else if (!fromTrans)
        {
            CStation *newSta = AddStation(from->line, to->name, true, true, 0, 0, to);
            return from->AddLink(newSta, dist, pass, mainWindow, charges, bothway);
        }
    }
    else
    {
        if (from->line == to->line)
        {
            return from->AddLink(to, dist, pass, mainWindow, charges, bothway);
        }
        else if (fromTrans)
        {
            CIntersite *link = from->link;
            CStation *newSta = NULL;
            while (from->transfer && link != NULL)
            {
                if (link->distance == 0 && link->station->line == to->line)
                {
                    newSta = link->station;
                    break;
                }
                link = link->link;
            }
            if (newSta == NULL)
            {
                newSta = AddStation(to->line, from->name, true, true, 0, 0, from);
            }
            return newSta->AddLink(to, dist, pass, mainWindow, charges, bothway);
        }
        else if (!fromTrans)
        {
            CIntersite *link = to->link;
            CStation *newSta = NULL;
            while (to->transfer && link != NULL)
            {
                if (link->distance == 0 && link->station->line == from->line)
                {
                    newSta = link->station;
                    break;
                }
                link = link->link;
            }
            if (newSta == NULL)
            {
                newSta = AddStation(from->line, to->name, true, true, 0, 0, to);
            }
            return from->AddLink(newSta, dist, pass, mainWindow, charges, bothway);
        }
    }
}

//获取线路总数
int CSubwayMap::GetNumOfLine() const
{
    return numOfLine;
}

//获取线路
CLine *CSubwayMap::GetLine(const int &index) const
{
    return line[index];
}

//获取站点
CStation *CSubwayMap::GetStation(const int &lineNum, const int &staNum) const
{
    return this->line[lineNum]->station[staNum];
}

//获取站点连接
CIntersite *CSubwayMap::GetLink(const int &lineNum, const int &staNum) const
{
    return this->line[lineNum]->station[staNum]->link;
}

//读取文件
void CSubwayMap::FromJson()
{
    QFile file("./subway.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    const QString &subway = file.readAll();
    file.close();
    QJsonParseError error;
    const QJsonDocument &document = QJsonDocument::fromJson(subway.toUtf8(), &error);
    if (QJsonParseError::NoError == error.error)
    {
        const QJsonObject &mapObject = document.object();
        const QString &announcement = mapObject.value("announcement").toString();
        mainWindow->announcement = announcement;
        const QJsonArray &lineArray = mapObject.value("line").toArray();
        SeqList<SeqList<QJsonArray>> intersite;
        SeqList<QJsonArray> station;
        for (int i = 0; i < lineArray.size(); i++)
        {
            const QJsonObject &lineObject = lineArray[i].toObject();
            const QString &lineName = lineObject.value("lineName").toString();
            const QString &lineColor = lineObject.value("lineColor").toString();
            const int &charges = lineObject.value("charges").toInt();
            const QString &firstTime = lineObject.value("firstTime").toString();
            const QString &lastTime = lineObject.value("lastTime").toString();
            AddLine(lineName, QColor(lineColor), charges, firstTime, lastTime);
            const QJsonArray &stationArray = lineObject.value("station").toArray();
            station.Add(stationArray);
            SeqList<QJsonArray> link;
            for (int j = 0; j < stationArray.size(); j++)
            {
                const QJsonObject &stationObject = stationArray[j].toObject();
                const QString &stationName = stationObject.value("name").toString();
                const bool &transfer = (bool)stationObject.value("transfer").toInt();
                const bool &entrance = (bool)stationObject.value("entrance").toInt();
                const bool &exit = (bool)stationObject.value("exit").toInt();
                const double &lng = stationObject.value("longitude").toDouble();
                const double &lat = stationObject.value("latitude").toDouble();
                QJsonArray linkArray = stationObject.value("link").toArray();
                link.Add(linkArray);
                line[i]->AddStation(stationName, entrance, exit, lng, lat, transfer);
                numOfStation++;
            }
            intersite.Add(link);
        }
        for (int i = 0; i < intersite.GetSize(); i++)
        {
            for (int j = 0; j < intersite[i].GetSize(); j++)
            {
                CStation *fromStation = line[i]->station[j];
                for (int k = 0; k < intersite[i][j].size(); k++)
                {
                    const QJsonObject &link = intersite[i][j][k].toObject();
                    const int &lineNum = link.value("line").toInt();
                    const int &distance = link.value("distance").toInt();
                    const int &toIndex = link.value("index").toInt();
                    const bool &passable = (bool)link.value("passable").toInt();
                    const int &charges = link.value("charges").toInt();
                    CStation *toStation = line[lineNum]->station[toIndex];
                    fromStation->AddLink(toStation, distance, passable, NULL, charges);
                    if (passable && distance == 0)
                    {
                        line[i]->AddLinkLine(line[lineNum]);
                    }
                }
            }
        }
        return;
    }
    throw error.errorString();
}

//保存文件
void CSubwayMap::ToJson()
{
    QJsonObject mapObject;
    QJsonArray lineArray;
    for (int i = 0; i < numOfLine; i++)
    {
        CLine *line = this->line[i];
        QJsonObject lineObject;
        lineObject.insert("lineName", line->name);
        lineObject.insert("lineColor", line->lineColor.name());
        lineObject.insert("numOfStation", line->numOfStation);
        lineObject.insert("charges", line->charges);
        lineObject.insert("firstTime", line->firstTime);
        lineObject.insert("lastTime", line->lastTime);
        QJsonArray stationArray;
        for (int j = 0; j < line->numOfStation; j++)
        {
            CStation *station = line->station[j];
            QJsonObject stationObject;
            stationObject.insert("line", station->line);
            stationObject.insert("name", station->name);
            stationObject.insert("entrance", (int)station->entrance);
            stationObject.insert("exit", (int)station->exit);
            stationObject.insert("transfer", (int)station->transfer);
            stationObject.insert("longitude", station->longitude);
            stationObject.insert("latitude", station->latitude);
            QJsonArray linkArray;
            CIntersite *intersite = station->link;
            while (intersite != NULL)
            {
                CStation *linkStation = intersite->station;
                QJsonObject link;
                link.insert("line", linkStation->line);
                link.insert("index", linkStation->index);
                link.insert("name", linkStation->name);
                link.insert("distance", intersite->distance);
                link.insert("passable", (int)intersite->passable);
                link.insert("charges", intersite->charges);
                linkArray.append(link);
                intersite = intersite->link;
            }
            stationObject.insert("link", linkArray);
            stationArray.append(stationObject);
        }
        lineObject.insert("station", stationArray);
        lineArray.append(lineObject);
    }
    mapObject.insert("line", lineArray);
    mapObject.insert("announcement", mainWindow->announcement);
    QString subway = QJsonDocument(mapObject).toJson();
    QFile file;
    file.setFileName("./subway.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream in(&file);
    in.setCodec("UTF-8");
    in << subway << endl;
    file.close();
}

//通过名称搜索线路
CLine *CSubwayMap::SearchLine(const QString &lineName) const
{
    for (int i = 0; i < numOfLine; i++)
    {
        if (line[i]->name == lineName)
        {
            return line[i];
        }
    }
    return NULL;
}

//通过名称搜索站点
CStation *CSubwayMap::SearchStation(const QString &staName) const
{
    for (int i = 0; i < numOfLine; i++)
    {
        for (int j = 0; j < line[i]->numOfStation; j++)
        {
            CStation *sta = line[i]->station[j];
            if (sta->name == staName)
            {
                return sta;
            }
        }
    }
    return NULL;
}

//查询所有线路
void CSubwayMap::FindAllPaths(CStation *const from, CStation *const to) const
{
    if (from == to)
    {
        return;
    }
    Stack<Node> stack;
    SeqList<Node> path;
    SeqList<SeqList<Node>> paths;
    SeqList<SeqList<int>> staStatus;
    SeqList<SeqList<int>> stackStatus;
    for (int i = 0; i < numOfLine; i++)
    {
        SeqList<int> sta;
        for (int j = 0; j < line[i]->numOfStation; j++)
        {
            sta.Add(0);
        }
        staStatus.Add(sta);
        stackStatus.Add(sta);
    }
    stack.Push(Node(0, NULL, from));

    staStatus[from->line][from->index] = 1;
    stackStatus[from->line][from->index] = 1;
    if (from->transfer)
    {
        CIntersite *staLink = from->link;
        while (staLink != NULL)
        {
            if (staLink->distance == 0)
            {
                staStatus[staLink->station->line][staLink->station->index] = 1;
            }
            staLink = staLink->link;
        }
    }
    DepthFirstSearch(from, to, stack, path, staStatus, stackStatus, paths);
    mainWindow->allFrom = from;
    mainWindow->allPaths = paths;
}

//DFS搜索所有线路
void CSubwayMap::DepthFirstSearch(CStation *const from, CStation *const to,
                                  Stack<Node> &stack,
                                  SeqList<Node> &path,
                                  SeqList<SeqList<int>> &staStatus,
                                  SeqList<SeqList<int>> &stackStatus,
                                  SeqList<SeqList<Node>> &paths) const
{
    CIntersite *link = from->link;
    while (link != NULL)
    {
        if (link->passable)
        {
            CStation *sta = link->station;
            if (link->distance == 0 && stackStatus[sta->line][sta->index] == 1)
            {
                link = link->link;
                continue;
            }
            if (link->distance != 0 && staStatus[sta->line][sta->index] == 1)
            {
                link = link->link;
                continue;
            }
            stack.Push(Node(0, link, sta));
            if (sta->transfer)
            {
                CIntersite *staLink = sta->link;
                while (staLink != NULL)
                {
                    if (staLink->distance == 0)
                    {
                        staStatus[staLink->station->line][staLink->station->index] = 1;
                    }
                    staLink = staLink->link;
                }
            }
            staStatus[sta->line][sta->index] = 1;
            stackStatus[sta->line][sta->index] = 1;
            if (stack.GetTop().station == to)
            {
                while (stack.NotEmpty())
                {
                    path.Add(stack.Pop());
                }
                for (int i = path.GetSize() - 1; i > 0; i--)
                {
                    stack.Push(path[i]);
                }
                paths.Add(path);
                path.Clear();
            }
            else
            {
                DepthFirstSearch(sta, to, stack, path, staStatus, stackStatus, paths);
            }
            stackStatus[sta->line][sta->index] = 0;
            if (sta->transfer)
            {
                bool inStack = false;
                CIntersite *staLink = sta->link;
                while (staLink != NULL)
                {
                    if (staLink->distance == 0)
                    {
                        if (stackStatus[staLink->station->line][staLink->station->index] == 1)
                        {
                            inStack = true;
                            break;
                        }
                    }
                    staLink = staLink->link;
                }
                if (inStack == false)
                {
                    staStatus[sta->line][sta->index] = 0;
                    staLink = sta->link;
                    while (staLink != NULL)
                    {
                        if (staLink->distance == 0)
                        {
                            staStatus[staLink->station->line][staLink->station->index] = 0;
                        }
                        staLink = staLink->link;
                    }
                }
            }
            else
            {
                staStatus[sta->line][sta->index] = 0;
            }
        }
        link = link->link;
    }
    stack.Pop();
}

//查询最小换乘
void CSubwayMap::FindMinTransferPath(CStation *from, CStation *to) const
{
    SeqList<CLine *> fromLine;
    SeqList<CLine *> toLine;
    SeqList<CStation *> fromSta;
    SeqList<CStation *> toSta;
    fromLine.Add(line[from->line]);
    fromSta.Add(from);
    toLine.Add(line[to->line]);
    toSta.Add(to);
    if (from->transfer)
    {
        CIntersite *link = from->link;
        while (link != NULL)
        {
            if (link->passable && link->distance == 0)
            {
                fromLine.Add(line[link->station->line]);
                fromSta.Add(link->station);
            }
            link = link->link;
        }
    }
    if (to->transfer)
    {
        CIntersite *link = to->link;
        while (link != NULL)
        {
            if (link->passable && link->distance == 0)
            {
                toLine.Add(line[link->station->line]);
                toSta.Add(link->station);
            }
            link = link->link;
        }
    }
    Stack<CLine *> stack;
    SeqList<SeqList<CLine *>> links;
    SeqList<int> lineStatus;
    for (int i = 0; i < numOfLine; i++)
    {
        lineStatus.Add(0);
    }
    int minTrans = numOfLine - 1;
    for (int i = 0; i < fromLine.GetSize(); i++)
    {
        CLine *line_1 = fromLine[i];
        for (int j = 0; j < toLine.GetSize(); j++)
        {
            CLine *line_2 = toLine[j];
            if (line_1 != line_2)
            {
                if (minTrans == 0)
                {
                    continue;
                }
                stack.Push(line_1);
                lineStatus[line_1->index] = 1;
                DepthFirstSearch(line_1, line_2, stack, links, lineStatus, minTrans);
                lineStatus[line_1->index] = 0;
            }
            else
            {
                SeqList<CLine *> link;
                link.Add(line_2);
                links.Add(link);
                minTrans = 0;
            }
        }
    }
    int minDist = INF;
    SeqList<SeqList<Node>> minTransferPath;
    CStation *finalToSta = to;
    CIntersite *finalToLink;
    CIntersite *toLink;
    SeqList<SeqList<Node>> tmp;
    for (int i = 0; i < links.GetSize(); i++)
    {
        if (links[i].GetSize() == minTrans + 1)
        {
            for (int j = 0; j < fromLine.GetSize(); j++)
            {
                if (links[i][minTrans] == fromLine[j])
                {
                    from = fromSta[j];
                }
            }
            for (int j = 0; j < toLine.GetSize(); j++)
            {
                if (links[i][0] == toLine[j])
                {
                    to = toSta[j];
                }
            }
            int dist = Dijkstra(from, to, links[i], tmp, &toLink);
            if (dist < minDist)
            {
                minDist = dist;
                minTransferPath = tmp;
                finalToSta = to;
                finalToLink = toLink;
            }
            tmp.Clear();
        }
    }
    mainWindow->FindShortestPath(finalToSta, minTransferPath);
    mainWindow->PaintPath(finalToSta, finalToLink);
    Dijkstra(from, to);
}

//DFS搜索线路连接
void CSubwayMap::DepthFirstSearch(CLine *const line_1, CLine *const line_2,
                                  Stack<CLine *> &stack, SeqList<SeqList<CLine *>> &links,
                                  SeqList<int> &lineStatus, int &minTrans) const
{
    for (int i = 0; i < line_1->linkLine.GetSize(); i++)
    {
        CLine *line = line_1->linkLine[i];
        if (!lineStatus[line->index])
        {
            stack.Push(line);
            lineStatus[line->index] = 1;
            if (stack.GetTop() == line_2)
            {
                if (minTrans > stack.GetSize() - 1)
                {
                    minTrans = stack.GetSize() - 1;
                }
                SeqList<CLine *> link;
                while (stack.NotEmpty())
                {
                    link.Add(stack.Pop());
                }
                for (int j = link.GetSize() - 1; j > 0; j--)
                {
                    stack.Push(link[j]);
                }
                links.Add(link);
                lineStatus[line->index] = 0;
                break;
            }
            else if (stack.GetSize() <= minTrans)
            {
                DepthFirstSearch(line, line_2, stack, links, lineStatus, minTrans);
            }
            else
            {
                stack.Pop();
            }
            lineStatus[line->index] = 0;
        }
    }
    stack.Pop();
}

//查询最小换乘情况下的最短路径
int CSubwayMap::Dijkstra(CStation *const from, CStation *const to,
                         const SeqList<CLine *> &trans, SeqList<SeqList<Node>> &paths,
                         CIntersite **toLink) const
{
    PriorityQueue<Node> queue;
    SeqList<SeqList<int>> inqueue, dists;
    for (int i = 0; i < numOfLine; i++)
    {
        SeqList<int> dist;
        SeqList<int> in;
        SeqList<Node> path;
        for (int j = 0; j < line[i]->numOfStation; j++)
        {
            dist.Add(INF);
            in.Add(0);
            path.Add(Node(0, 0, 0));
        }
        dists.Add(dist);
        inqueue.Add(in);
        paths.Add(path);
    }
    queue.Push(Node(0, NULL, from));
    dists[from->line][from->index] = 0;
    while (!queue.Empty())
    {
        Node top = queue.GetTop();
        queue.Pop();
        if (top.station == to)
        {
            *toLink = top.link;
            break;
        }
        CStation *curr = top.station;
        if (!inqueue[curr->line][curr->index])
        {
            inqueue[curr->line][curr->index] = 1;
            CIntersite *link = curr->link;
            while (link != NULL)
            {
                CStation *toSta = link->station;
                if (link->distance == 0)
                {
                    CLine *transLine = line[link->station->line];
                    bool find = false;
                    for (int i = 0; i < trans.GetSize() - 1; i++)
                    {
                        if (transLine == trans[i])
                        {
                            find = true;
                            break;
                        }
                    }
                    if (!find)
                    {
                        link = link->link;
                        continue;
                    }
                }
                if (link->passable && top.dist + link->distance < dists[toSta->line][toSta->index])
                {
                    dists[toSta->line][toSta->index] = top.dist + link->distance;
                    queue.Push(Node(dists[toSta->line][toSta->index], link, toSta));
                    paths[toSta->line][toSta->index] = top;
                }
                link = link->link;
            }
        }
    }
    return dists[to->line][to->index];
}

//查询最短路径
void CSubwayMap::Dijkstra(CStation *const from, CStation *const to, const bool &minPath) const
{
    if (from == to)
    {
        return;
    }
    PriorityQueue<Node> queue;
    SeqList<SeqList<int>> inqueue, dists;
    SeqList<SeqList<Node>> paths;
    CIntersite *toLink;
    for (int i = 0; i < numOfLine; i++)
    {
        SeqList<int> dist;
        SeqList<int> in;
        SeqList<Node> path;
        for (int j = 0; j < line[i]->numOfStation; j++)
        {
            dist.Add(INF);
            in.Add(0);
            path.Add(Node(0, 0, 0));
        }
        dists.Add(dist);
        inqueue.Add(in);
        paths.Add(path);
    }
    queue.Push(Node(0, NULL, from));
    dists[from->line][from->index] = 0;
    while (!queue.Empty())
    {
        Node top = queue.GetTop();
        queue.Pop();
        if (top.station == to)
        {
            toLink = top.link;
            break;
        }
        CStation *curr = top.station;
        if (!inqueue[curr->line][curr->index])
        {
            inqueue[curr->line][curr->index] = 1;
            CIntersite *link = curr->link;
            while (link != NULL)
            {
                CStation *toSta = link->station;
                if (minPath && link->passable && top.dist + link->distance < dists[toSta->line][toSta->index])
                {
                    dists[toSta->line][toSta->index] = top.dist + link->distance;
                    queue.Push(Node(dists[toSta->line][toSta->index], link, toSta));
                    paths[toSta->line][toSta->index] = top;
                }
                else if (!minPath && top.dist + link->distance < dists[toSta->line][toSta->index])
                {
                    dists[toSta->line][toSta->index] = top.dist + link->distance;
                    queue.Push(Node(dists[toSta->line][toSta->index], link, toSta));
                    paths[toSta->line][toSta->index] = top;
                }
                link = link->link;
            }
        }
    }
    mainWindow->FindShortestPath(to, paths);
    if (minPath == true)
    {
        mainWindow->PaintPath(to, toLink);
        Dijkstra(from, to);
    }
    else
    {
        mainWindow->CountCharges(toLink);
    }
}

//析构函数
CSubwayMap::~CSubwayMap()
{
    for (int i = 0; i < numOfLine; i++)
    {
        for (int j = 0; j < line[i]->numOfStation; j++)
        {
            CIntersite *ptr = line[i]->station[j]->link;
            while (ptr)
            {
                CIntersite *next = ptr->link;
                delete ptr;
                ptr = next;
            }
            delete line[i]->station[j];
        }
        delete line[i];
    }
}
