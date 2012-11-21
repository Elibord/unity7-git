// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/*
 * Copyright 2012 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3, as
 * published by the  Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the applicable version of the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of both the GNU Lesser General Public
 * License version 3 along with this program.  If not, see
 * <http://www.gnu.org/licenses/>
 *
 * Authored by: Manuel de la Pena <manuel.delapena@canonical.com>
 *
 */

#ifndef MUSIC_PAYMENT_PREVIEW_H
#define MUSIC_PAYMENT_PREVIEW_H

// key used to find the correct info hint
#define DATA_INFOHINT_ID "album_purchase_preview"

// keys of the data preview
#define DATA_TITLE_KEY "title"
#define DATA_SUBTITLE_KEY "subtitle"
#define DATA_HEADER_KEY "header"
#define DATA_EMAIL_LABEL_KEY "email_label"
#define DATA_EMAIL_KEY "email"
#define DATA_PAYMENT_LABEL_KEY "payment_label"
#define DATA_PAYMENT_METHOD_KEY "payment_method"
#define DATA_PASSWORD_LABEL_KEY "password_label"
#define DATA_PASSWORD_HINT_KEY "password_hint"
#define DATA_PURCHASE_HINT_KEY "purchase_hint"
#define DATA_PURCHASE_PRIZE_KEY "purchase_price"
#define DATA_PURCHASE_TYPE_KEY "purchase_type"
#define DATA_PASSWORD_KEY "password"

// ations ids
#define CHANGE_PAYMENT_ACTION "change_payment_method"
#define FORGOT_PASSWORD_ACTION "forgot_password"
#define CANCEL_PURCHASE_ACTION "cancel_purchase"
#define PURCHASE_ALBUM_ACTION "purchase_album"

#include <Nux/Nux.h>
#include <Nux/AbstractButton.h>
#include <UnityCore/Lens.h>
#include <UnityCore/MusicPaymentPreview.h>
#include "ActionButton.h"
#include "ActionLink.h"
#include "PaymentPreview.h"
#include "unity-shared/IconTexture.h"
#include "unity-shared/TextInput.h"

namespace nux
{
class AbstractPaintLayer;
class StaticCairoText;
class VLayout;
}

namespace unity
{
namespace dash
{
namespace previews
{
class CoverArt;
class PreviewInfoHintWidget;

class MusicPaymentPreview : public PaymentPreview
{
public:
  typedef nux::ObjectPtr<MusicPaymentPreview> Ptr;
  NUX_DECLARE_OBJECT_TYPE(MusicPaymentPreview, Preview);

  MusicPaymentPreview(dash::Preview::Ptr preview_model);
  ~MusicPaymentPreview();

  nux::Area* FindKeyFocusArea(unsigned int key_symbol,
                              unsigned long x11_key_code,
                              unsigned long special_keys_state);
  // From debug::Introspectable
  std::string GetName() const;
  nux::Layout* GetTitle();
  nux::Layout* GetPrize();
  nux::Layout* GetBody();
  nux::Layout* GetFormLabels();
  nux::Layout* GetFormFields();
  nux::Layout* GetFormActions();
  nux::Layout* GetEmail();
  nux::Layout* GetPayment();
  nux::Layout* GetPassword();
  nux::Layout* GetFooter();

private:
  std::string GetDataForKey(GVariant *dict, std::string key);
  void LoadActions();

protected:
  void OnActionActivated(ActionButton* button, std::string const& id);
  void OnActionLinkActivated(ActionLink* link, std::string const& id);

  void PreLayoutManagement();

protected:
  // content elements
  nux::ObjectPtr<CoverArt> image_;
  nux::ObjectPtr<nux::StaticCairoText> title_;
  nux::ObjectPtr<nux::StaticCairoText> subtitle_;
  nux::ObjectPtr<nux::StaticCairoText> email_label_;
  nux::ObjectPtr<nux::StaticCairoText> email_;
  nux::ObjectPtr<nux::StaticCairoText> payment_label_;
  nux::ObjectPtr<nux::StaticCairoText> payment_;
  nux::ObjectPtr<nux::StaticCairoText> password_label_;
  nux::ObjectPtr<TextInput> password_entry_;
  nux::ObjectPtr<nux::StaticCairoText> purchase_hint_;
  nux::ObjectPtr<nux::StaticCairoText> purchase_prize_;
  nux::ObjectPtr<nux::StaticCairoText> purchase_type_;
  nux::ObjectPtr<nux::StaticCairoText> change_payment_;
  nux::ObjectPtr<nux::StaticCairoText> forgotten_password_;

  // do we want to type?
  bool entry_selected_;

  // actions
  std::map<std::string, nux::ObjectPtr<nux::AbstractButton>> sorted_buttons_;

  // lock texture
  IconTexture* lock_texture_;

  typedef std::unique_ptr<nux::AbstractPaintLayer> LayerPtr;
  LayerPtr details_bg_layer_;
};

}
}
}

#endif // MUSIC_PAYMENT_PREVIEW_H
