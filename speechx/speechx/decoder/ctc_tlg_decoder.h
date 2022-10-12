// Copyright (c) 2022 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "base/common.h"
#include "decoder/decoder_itf.h"

#include "kaldi/decoder/lattice-faster-online-decoder.h"
#include "util/parse-options.h"

namespace ppspeech {

struct TLGDecoderOptions {
    kaldi::LatticeFasterDecoderConfig opts;
    // todo remove later, add into decode resource
    std::string word_symbol_table;
    std::string fst_path;

    TLGDecoderOptions() : word_symbol_table(""), fst_path("") {}
};

class TLGDecoder : public DecoderInterface {
  public:
    explicit TLGDecoder(TLGDecoderOptions opts);
    ~TLGDecoder() = default;

    void InitDecoder();
    void Reset();

    void AdvanceDecode(
        const std::shared_ptr<kaldi::DecodableInterface>& decodable);


    std::string GetFinalBestPath();
    std::string GetPartialResult();


    void Decode();

    std::string GetBestPath();
    std::vector<std::pair<double, std::string>> GetNBestPath();

    int NumFrameDecoded();
    int DecodeLikelihoods(const std::vector<std::vector<BaseFloat>>& probs,
                          std::vector<std::string>& nbest_words);


  private:
    void AdvanceDecoding(kaldi::DecodableInterface* decodable);

    std::shared_ptr<kaldi::LatticeFasterOnlineDecoder> decoder_;
    std::shared_ptr<fst::Fst<fst::StdArc>> fst_;
    std::shared_ptr<fst::SymbolTable> word_symbol_table_;
    // the frame size which have decoded starts from 0.
    int32 num_frame_decoded_;
};


}  // namespace ppspeech