#include "feature.h"

#include "context.h"
#include <typeinfo>
#include <QDebug>

Features::Features(QString path, bool single): Symbol("features")
{
  FeaturesParser parser(path);
  m_ds = parser.parse();

  if (single) {
    QList<Record*> records = m_ds->records();

    for (QList<Record*>::const_iterator it = records.begin();
        it != records.end(); ++it) {
      Record* rec = *it;
      rec->addToChild(this);
    }
  }
}

Features::~Features()
{
  delete m_ds;
}

void Features::addToScene(QGraphicsScene* scene)
{
  QList<Record*> records = m_ds->records();

  for (QList<Record*>::const_iterator it = records.begin();
      it != records.end(); ++it) {
    Record* rec = *it;
    rec->addToScene(scene);
  }
}

QTableWidget* Features::symbolCount()
{
    QTextEdit *output = new QTextEdit;
    FeaturesDataStore::IDMapType nameMap;
    int total;
    total = 0;
    nameMap = m_ds->symbolNameMap();

    total += createSection(output,"Line",nameMap);
    total += createSection(output,"Pad",nameMap);
    total += createSection(output,"Arc",nameMap);
    total += createSection(output,"Surface",nameMap);
    total += createSection(output,"Text",nameMap);

    output->append("\n--------------------------------------");
    output->append(QString().sprintf("Total \t %20s \t %d"," ",total));
    output->setReadOnly(true);
    return output;
}

int Features::createSection(QTextEdit *output,
            QString sectionTitle, FeaturesDataStore::IDMapType nameMap)
{
    FeaturesDataStore::CountMapType posCountMap,negCountMap;
    QString text;
    int local_total;
    local_total = 0;
    output->append(sectionTitle+
                   " List\n--------------------------------------");

    if(sectionTitle == "Line"){
        posCountMap = m_ds->posLineCountMap();
        negCountMap = m_ds->negLineCountMap();
    }else if(sectionTitle == "Pad"){
        posCountMap = m_ds->posPadCountMap();
        negCountMap = m_ds->negPadCountMap();
    }else if(sectionTitle == "Arc"){
        posCountMap = m_ds->posArcCountMap();
        negCountMap = m_ds->negArcCountMap();
    }else if(sectionTitle == "Surface"){
        text.sprintf("POS \t %20s \t %d"," ",
                     (local_total+=m_ds->posSurfaceCount()));
        output->append(text);
        text.sprintf("NEG \t %20s \t %d"," ",
                     (local_total+=m_ds->negSurfaceCount()));
        output->append(text);
        output->append(" ");
        return local_total;
    }else if(sectionTitle == "Text"){
        text.sprintf("POS \t %20s \t %d"," ",
                     (local_total+=m_ds->posTextCount()));
        output->append(text);
        text.sprintf("NEG \t %20s \t %d"," ",
                     (local_total+=m_ds->negTextCount()));
        output->append(text);
        output->append(" ");
        return local_total;
    }


    for(int i = 0;i < posCountMap.size();i++){
      if(posCountMap[i] != 0){
        text.sprintf("POS \t %20s \t %d",
                     nameMap[i].toAscii().data(),posCountMap[i]);
        output->append(text);
        local_total+=posCountMap[i];
      }
    }
    for(int i = 0;i < negCountMap.size();i++){
      if(negCountMap[i] != 0){
        text.sprintf("NEG \t %20s \t %d",
                     nameMap[i].toAscii().data(),negCountMap[i]);
        output->append(text);
        local_total+=negCountMap[i];
      }
    }
    output->append(text.sprintf("Total \t %20s \t %d","",local_total));
    output->append(" ");
    return local_total;
}
