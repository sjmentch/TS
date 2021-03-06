/* Copyright (C) 2010 Ion Torrent Systems, Inc. All Rights Reserved */
#ifndef EVALUATEKEY_H
#define EVALUATEKEY_H

#include <vector>
#include <string>

#include "TraceStoreCol.h"

class EvaluateKey {

 public:

  EvaluateKey();

  ~EvaluateKey() { Cleanup(); }

  void Init();

  void Alloc(size_t num_well_flows, size_t num_frames, size_t num_flows, int num_wells);

  void Cleanup();

  void SetSizes(int row_start, int row_end, 
                int col_start, int col_end,
                int flow_start, int flow_end,
                int frame_start, int frame_end);
    
  void SetUpMatrices(TraceStoreCol &trace_store, 
                     int col_stride, int flow_stride,
                     int row_start, int row_end, int col_start, int col_end,
                     int flow_start, int flow_end,
                     int frame_start, int frame_end,
                     float *trace_data,
                     float *ref_data);

  void SetUpMatrices(TraceStoreCol &trace_store, 
                     int col_stride, int flow_stride,
                     int row_start, int row_end, int col_start, int col_end,
                     int flow_start, int flow_end,
                     int frame_start, int frame_end) {
    SetUpMatrices(trace_store, col_stride, flow_stride,
                  row_start, row_end, col_start, col_end,
                  flow_start, flow_end, frame_start, frame_end,
                  m_trace_data, m_ref_data);
  }

  static void FitTauB(const int *zero_flows, size_t n_zero_flows, 
                      const float *trace_data, const float *ref_data, 
                      size_t n_wells, size_t n_flows, size_t n_flow_wells,
                      size_t n_frames, float *__restrict taub);

  static void PredictZeromersSignal(const float *time, int n_frames,
                                    float *trace, float *ref, float *zeromer,
                                    size_t n_wells, size_t n_flows, 
                                    size_t n_flow_wells, 
                                    float taue_est, float *__restrict taub);

  static void ZeromerSumSqError(const int *zero_flows, size_t n_zero_flows, 
                                float *signal_data, 
                                size_t n_wells, size_t n_flows, 
                                size_t n_flow_wells, size_t n_frames, 
                                float taue_est, float *__restrict taub,
                                double &ssq);

  static void ZeromerMadError(const int *zero_flows, size_t n_zero_flows, 
                              float *signal_data, 
                              size_t n_wells, size_t n_flows, 
                              size_t n_flow_wells, size_t n_frames, 
                              float taue_est, float *__restrict taub,
                              double &mad);


  void FitTauB(KeySeq &key, const float *time, float taue_est, int frame_start, int frame_end, float *__restrict taub);

  void PredictZeromersVec(const float *time, float taue_est, float * __restrict taub);


  void ScoreKeySignals(KeySeq &key, float *__restrict key_signal_ptr, 
                       int integration_start, int integration_end,
                       int peak_start, int peak_end,
                       float *onemer_norm,
                       float *__restrict key_results_ptr, int num_result_cols, float *__restrict taub, float taue_est);

  void FindBestKey(int row_start, int row_end, int col_start, int col_end,
                   int frame_start, int frame_end,
                   int flow_stride, int col_stride, 
                   const float *time, std::vector<KeySeq> &keys, 
                   float shift, float taue_est, char *bad_wells,
                   std::vector<KeyFit> &key_fits);

  void Calculate1MerNormalization(size_t num_flows, size_t num_frames,
                                  std::vector<int> &onemer_flows,
                                  float *flow_1mer_avg,int *flow_1mer_count,
                                  float *norm_factors,
                                  size_t integration_start, size_t integration_end);


  bool m_debug;
  // Options to change the behavior
  bool m_doing_darkmatter, m_use_projection, m_peak_signal_frames, m_integration_width, m_normalize_1mers;
  // Matrices of data with frame major, then flow, then column major indexing
  float *m_trace_data, *m_ref_data, *m_zeromer_est, *m_shifted_ref;
  // Per frame average and spread for 1mers and 0mers
  float *m_avg_0mer, *m_sd_0mer, *m_avg_1mer, *m_sd_1mer;
  float *m_flow_avg_1mer;
  int *m_flow_count_1mer;
  std::vector<int> m_flow_order;
  std::vector<float> m_flow_key_avg;
  std::vector<int> m_key_counts;
  size_t m_min_integration;
  size_t m_integration_start, m_integration_end; 
  size_t m_peak_start, m_peak_end;
  size_t m_num_wells, m_num_well_flows, m_num_frames, m_num_flows;

};

#endif // EVALUATEKEY_H
