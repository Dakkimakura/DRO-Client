#include "aoscene.h"

#include "aoapplication.h"
#include "file_functions.h"

#include <QMovie>

AOScene::AOScene(QWidget *parent, AOApplication *p_ao_app) : QLabel(parent), ao_app(p_ao_app)
{
  m_reader = new QMovie(this);
  setMovie(m_reader);
}

void AOScene::set_image(QString p_image)
{
  const QString l_file_path =
      ao_app->find_asset_path(ao_app->get_current_background_path() + "/" + p_image, animated_or_static_extensions());

  // do not update the movie if we're using the same file
  if (m_reader->fileName() == l_file_path)
    return;

  m_reader->stop();
  delete m_reader;

  m_reader = new QMovie(this);
  m_reader->setScaledSize(size());
  m_reader->setFileName(l_file_path);
  setMovie(m_reader);
  m_reader->start();
}

void AOScene::combo_resize(QSize p_size)
{
  resize(p_size);
  m_reader->stop();
  m_reader->setScaledSize(p_size);
  m_reader->start();
}
