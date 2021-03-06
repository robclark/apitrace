/**************************************************************************
 *
 * Copyright 2017 Intel Corporation
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Authors:
 *   Mark Janes <mark.a.janes@intel.com>
 **************************************************************************/

#ifndef _GLFRAME_EXPERIMENT_MODEL_HPP_
#define _GLFRAME_EXPERIMENT_MODEL_HPP_

#include <QObject>
#include <QString>

#include <mutex>
#include <string>
#include <map>
#include <vector>

#include "glframe_retrace_interface.hpp"
#include "glframe_traits.hpp"

namespace glretrace {

// This class is highly similar to QApiModel.  Consider merging them
// and making the filter functionality optional.  For now, it should
// remain separate, as the feature is a proof-of-concept that may soon
// be deleted.
class FrameRetraceModel;
class QExperimentModel : public QObject,
                         NoCopy, NoAssign, NoMove{
  Q_OBJECT
  Q_PROPERTY(Qt::CheckState selectionDisabled READ selectionDisabled
             NOTIFY onDisabled)
  Q_PROPERTY(Qt::CheckState selectionSimpleShader READ selectionSimpleShader
             NOTIFY onSimpleShader)
  Q_PROPERTY(Qt::CheckState selectionScissorRect READ selectionScissorRect
             NOTIFY onScissorRect)
  Q_PROPERTY(Qt::CheckState selectionWireframe READ selectionWireframe
             NOTIFY onWireframe)
  Q_PROPERTY(Qt::CheckState selectionTexture2x2 READ selectionTexture2x2
             NOTIFY onTexture2x2)
 public:
  QExperimentModel();
  explicit QExperimentModel(IFrameRetrace *retrace);
  ~QExperimentModel() {}

  Qt::CheckState selectionDisabled() const { return m_disabled_checkbox; }
  Qt::CheckState selectionSimpleShader() const { return m_simple_checkbox; }
  Qt::CheckState selectionScissorRect() const { return m_scissor_checkbox; }
  Qt::CheckState selectionWireframe() const { return m_wireframe_checkbox; }
  Qt::CheckState selectionTexture2x2() const { return m_texture_2x2_checkbox; }

  void onSelect(SelectionId selection_count, const QList<int> &selection);
  void onRevert();

  Q_INVOKABLE void disableDraw(Qt::CheckState disable);
  Q_INVOKABLE void simpleShader(Qt::CheckState simple);
  Q_INVOKABLE void scissorRect(Qt::CheckState scissor);
  Q_INVOKABLE void wireframe(Qt::CheckState wireframe);
  Q_INVOKABLE void texture2x2(Qt::CheckState texture2x2);
 signals:
  void onDisabled();
  void onSimpleShader();
  void onScissorRect();
  void onWireframe();
  void onTexture2x2();
  void onExperiment();

 private:
  IFrameRetrace *m_retrace;
  std::map<RenderId, bool> m_disabled, m_simple, m_scissor, m_wireframe,
    m_texture_2x2;
  QList<int> m_selection;
  SelectionId m_count;
  Qt::CheckState m_disabled_checkbox, m_simple_checkbox, m_scissor_checkbox,
    m_wireframe_checkbox, m_texture_2x2_checkbox;
};

}  // namespace glretrace

#endif  // _GLFRAME_EXPERIMENT_MODEL_HPP_
