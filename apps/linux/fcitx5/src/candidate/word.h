//! 候选正文与第一条译词；选词仍回 Server 调 Engine::commit。
#pragma once
#include <fcitx/candidatelist.h>
#include <functional>
#include <string>
namespace qingjian {
class Word final : public fcitx::CandidateWord {
public:
    Word(std::string text, std::string comment, std::function<void(fcitx::InputContext *)> select)
        : CandidateWord(fcitx::Text(text)), select_(std::move(select)) {
        setPlaceHolder(text.empty());
#ifdef QINGJIAN_FCITX_CANDIDATE_COMMENT
        setComment(std::move(comment));
#else
        body_ = std::move(text);
        comment_ = fcitx::Text(std::move(comment));
        if (!comment_.empty()) {
            auto value = this->text();
            value.append(" ");
            value.append(comment_.toString());
            setText(std::move(value));
        }
#endif
    }
    void select(fcitx::InputContext *context) const override { if (!isPlaceHolder()) select_(context); }
#ifndef QINGJIAN_FCITX_CANDIDATE_COMMENT
    /// Fcitx5 5.1.9 之前没有基类译词 API，正文与译词由插件留底。
    const std::string &body() const { return body_; }
    const fcitx::Text &comment() const { return comment_; }
#endif
private:
    /// 含帧版本检查的选择操作。
    std::function<void(fcitx::InputContext *)> select_;
#ifndef QINGJIAN_FCITX_CANDIDATE_COMMENT
    /// Fcitx5 5.1.9 之前面板只画 CandidateWord::text()，按上游 textWithComment 的规则把译词并进正文，两份都留底。
    std::string body_;
    fcitx::Text comment_;
#endif
};

/// 候选正文：Fcitx5 5.1.9 之前 text() 里还并着译词。
inline std::string textOf(const fcitx::CandidateWord &word) {
#ifdef QINGJIAN_FCITX_CANDIDATE_COMMENT
    return word.text().toString();
#else
    return static_cast<const Word &>(word).body();
#endif
}

/// 候选译词：Fcitx5 5.1.9 之前没有基类访问器。
inline std::string commentOf(const fcitx::CandidateWord &word) {
#ifdef QINGJIAN_FCITX_CANDIDATE_COMMENT
    return word.comment().toString();
#else
    return static_cast<const Word &>(word).comment().toString();
#endif
}
}
