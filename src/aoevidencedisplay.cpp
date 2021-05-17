#include <QDebug>

#include "aoevidencedisplay.h"

#include "datatypes.h"
#include "file_functions.h"
#include "misc_functions.h"

AOEvidenceDisplay::AOEvidenceDisplay(QWidget *p_parent, AOApplication *p_ao_app) : QLabel(p_parent)
{
  ao_app = p_ao_app;

  m_movie = new QMovie(this);
  w_icon = new QLabel(this);
  sfx_player = new AOSfxPlayer(ao_app, this);

  connect(m_movie, SIGNAL(frameChanged(int)), this, SLOT(frame_change(int)));
}

void AOEvidenceDisplay::show_evidence(QString p_evidence_image, bool is_left_side)
{
  this->reset();

  QString f_evidence_path = ao_app->get_evidence_path(p_evidence_image);

  AOPixmap f_pixmap(f_evidence_path);

  QString final_gif_path;
  QString gif_name;
  QString icon_identifier;

  if (is_left_side)
  {
    icon_identifier = "left_evidence_icon";
    gif_name = "evidence_appear_left.gif";
  }
  else
  {
    icon_identifier = "right_evidence_icon";
    gif_name = "evidence_appear_right.gif";
  }

  pos_size_type icon_dimensions = ao_app->get_element_dimensions(icon_identifier, "courtroom_design.ini");

  w_icon->move(icon_dimensions.x, icon_dimensions.y);
  w_icon->resize(icon_dimensions.width, icon_dimensions.height);
  w_icon->setPixmap(f_pixmap.scale(w_icon->size()));

  QString f_path = ao_app->find_theme_asset_path(gif_name);
  m_movie->setFileName(f_path);
  if (m_movie->frameCount() < 1)
    return;

  this->setMovie(m_movie);

  m_loop_number = 0;
  m_movie->start();
  sfx_player->play_effect(ao_app->get_sfx("evidence_present"));
}

void AOEvidenceDisplay::frame_change(int p_frame_index)
{
  const int l_frame_num = p_frame_index + 1;
  if (l_frame_num < m_movie->frameCount())
    return;

  if ((p_frame_index + 1) < m_movie->frameCount())
    return;
  if (p_frame_index == (m_movie->frameCount() - 1))
  {
    QTimer::singleShot(m_movie->nextFrameDelay(), this, [this]() {
      m_movie->stop();
      w_icon->show();
      clear();
    });
    // we need this or else the last frame wont show
    delay(m_movie->nextFrameDelay());
    m_movie->stop();
    w_icon->show();
    clear();
  }
}

void AOEvidenceDisplay::reset()
{
  m_movie->stop();
  w_icon->hide();
  this->clear();
}
