/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Fri Jan 24 19:47:46 2020
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */



#include "network.h"

#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "layers.h"

#undef AI_TOOLS_VERSION_MAJOR
#undef AI_TOOLS_VERSION_MINOR
#undef AI_TOOLS_VERSION_MICRO
#define AI_TOOLS_VERSION_MAJOR 4
#define AI_TOOLS_VERSION_MINOR 1
#define AI_TOOLS_VERSION_MICRO 0


#undef AI_TOOLS_API_VERSION_MAJOR
#undef AI_TOOLS_API_VERSION_MINOR
#undef AI_TOOLS_API_VERSION_MICRO
#define AI_TOOLS_API_VERSION_MAJOR 1
#define AI_TOOLS_API_VERSION_MINOR 3
#define AI_TOOLS_API_VERSION_MICRO 0

#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network
 
#undef AI_NETWORK_MODEL_SIGNATURE
#define AI_NETWORK_MODEL_SIGNATURE     "e05115fb48b62400e7f042a0c5ee738e"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     "(rev-4.1.0)"
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Fri Jan 24 19:47:46 2020"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

/**  Forward network declaration section  *************************************/
AI_STATIC ai_network AI_NET_OBJ_INSTANCE;


/**  Forward network array declarations  **************************************/
AI_STATIC ai_array dense_bias_array;   /* Array #0 */
AI_STATIC ai_array dense_weights_array;   /* Array #1 */
AI_STATIC ai_array lstm_1_bias_array;   /* Array #2 */
AI_STATIC ai_array lstm_1_peephole_array;   /* Array #3 */
AI_STATIC ai_array lstm_1_recurrent_array;   /* Array #4 */
AI_STATIC ai_array lstm_1_kernel_array;   /* Array #5 */
AI_STATIC ai_array lstm_bias_array;   /* Array #6 */
AI_STATIC ai_array lstm_peephole_array;   /* Array #7 */
AI_STATIC ai_array lstm_recurrent_array;   /* Array #8 */
AI_STATIC ai_array lstm_kernel_array;   /* Array #9 */
AI_STATIC ai_array input_0_output_array;   /* Array #10 */
AI_STATIC ai_array lstm_output_array;   /* Array #11 */
AI_STATIC ai_array lstm_1_output_array;   /* Array #12 */
AI_STATIC ai_array dense_output_array;   /* Array #13 */


/**  Forward network tensor declarations  *************************************/
AI_STATIC ai_tensor dense_bias;   /* Tensor #0 */
AI_STATIC ai_tensor dense_weights;   /* Tensor #1 */
AI_STATIC ai_tensor lstm_1_bias;   /* Tensor #2 */
AI_STATIC ai_tensor lstm_1_peephole;   /* Tensor #3 */
AI_STATIC ai_tensor lstm_1_recurrent;   /* Tensor #4 */
AI_STATIC ai_tensor lstm_1_kernel;   /* Tensor #5 */
AI_STATIC ai_tensor lstm_bias;   /* Tensor #6 */
AI_STATIC ai_tensor lstm_peephole;   /* Tensor #7 */
AI_STATIC ai_tensor lstm_recurrent;   /* Tensor #8 */
AI_STATIC ai_tensor lstm_kernel;   /* Tensor #9 */
AI_STATIC ai_tensor input_0_output;   /* Tensor #10 */
AI_STATIC ai_tensor lstm_output;   /* Tensor #11 */
AI_STATIC ai_tensor lstm_1_output;   /* Tensor #12 */
AI_STATIC ai_tensor dense_output;   /* Tensor #13 */


/**  Forward network tensor chain declarations  *******************************/
AI_STATIC_CONST ai_tensor_chain lstm_chain;   /* Chain #0 */
AI_STATIC_CONST ai_tensor_chain lstm_1_chain;   /* Chain #1 */
AI_STATIC_CONST ai_tensor_chain dense_chain;   /* Chain #2 */


/**  Subgraph network operator tensor chain declarations  *********************/


/**  Subgraph network operator declarations  *********************************/


/**  Forward network layer declarations  **************************************/
AI_STATIC ai_layer_lstm lstm_layer; /* Layer #0 */
AI_STATIC ai_layer_lstm lstm_1_layer; /* Layer #1 */
AI_STATIC ai_layer_dense dense_layer; /* Layer #2 */


/**  Array declarations section  **********************************************/
AI_ARRAY_OBJ_DECLARE(
  dense_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  dense_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 30,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  lstm_1_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 120,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  lstm_1_peephole_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 90,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  lstm_1_recurrent_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3600,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  lstm_1_kernel_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3600,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  lstm_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 120,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  lstm_peephole_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 90,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  lstm_recurrent_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3600,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  lstm_kernel_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 120,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  input_0_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 3,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  lstm_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 90,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  lstm_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 30,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  dense_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 1,
  AI_STATIC)




/**  Tensor declarations section  *********************************************/
AI_TENSOR_OBJ_DECLARE(
  dense_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &dense_bias_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 30, 1, 1, 1), AI_STRIDE_INIT(4, 4, 120, 120, 120),
  1, &dense_weights_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  lstm_1_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 120, 1, 1), AI_STRIDE_INIT(4, 4, 4, 480, 480),
  1, &lstm_1_bias_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  lstm_1_peephole, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 90, 1, 1), AI_STRIDE_INIT(4, 4, 4, 360, 360),
  1, &lstm_1_peephole_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  lstm_1_recurrent, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 30, 1, 1, 120), AI_STRIDE_INIT(4, 4, 120, 120, 120),
  1, &lstm_1_recurrent_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  lstm_1_kernel, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 30, 1, 1, 120), AI_STRIDE_INIT(4, 4, 120, 120, 120),
  1, &lstm_1_kernel_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  lstm_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 120, 1, 1), AI_STRIDE_INIT(4, 4, 4, 480, 480),
  1, &lstm_bias_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  lstm_peephole, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 90, 1, 1), AI_STRIDE_INIT(4, 4, 4, 360, 360),
  1, &lstm_peephole_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  lstm_recurrent, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 30, 1, 1, 120), AI_STRIDE_INIT(4, 4, 120, 120, 120),
  1, &lstm_recurrent_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  lstm_kernel, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 1, 1, 120), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &lstm_kernel_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  input_0_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 1, 1, 3), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &input_0_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  lstm_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 30, 1, 3), AI_STRIDE_INIT(4, 4, 4, 120, 120),
  1, &lstm_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  lstm_1_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 30, 1, 1), AI_STRIDE_INIT(4, 4, 4, 120, 120),
  1, &lstm_1_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  dense_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &dense_output_array, NULL)


/**  Layer declarations section  **********************************************/



AI_TENSOR_CHAIN_OBJ_DECLARE(
  lstm_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&input_0_output),
  AI_TENSOR_LIST_ENTRY(&lstm_output),
  AI_TENSOR_LIST_ENTRY(&lstm_kernel, &lstm_recurrent, &lstm_peephole, &lstm_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  lstm_layer, 0,
  LSTM_TYPE,
  lstm, forward_lstm,
  &AI_NET_OBJ_INSTANCE, &lstm_1_layer, AI_STATIC,
  .tensors = &lstm_chain, 
  .n_units = 30, 
  .activation_nl = ai_math_tanh, 
  .recurrent_nl = ai_math_sigmoid, 
  .go_backwards = false, 
  .reverse_seq = false, 
  .out_nl = ai_math_tanh, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  lstm_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&lstm_output),
  AI_TENSOR_LIST_ENTRY(&lstm_1_output),
  AI_TENSOR_LIST_ENTRY(&lstm_1_kernel, &lstm_1_recurrent, &lstm_1_peephole, &lstm_1_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  lstm_1_layer, 2,
  LSTM_TYPE,
  lstm, forward_lstm,
  &AI_NET_OBJ_INSTANCE, &dense_layer, AI_STATIC,
  .tensors = &lstm_1_chain, 
  .n_units = 30, 
  .activation_nl = ai_math_tanh, 
  .recurrent_nl = ai_math_sigmoid, 
  .go_backwards = false, 
  .reverse_seq = false, 
  .out_nl = ai_math_tanh, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&lstm_1_output),
  AI_TENSOR_LIST_ENTRY(&dense_output),
  AI_TENSOR_LIST_ENTRY(&dense_weights, &dense_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_layer, 4,
  DENSE_TYPE,
  dense, forward_dense,
  &AI_NET_OBJ_INSTANCE, &dense_layer, AI_STATIC,
  .tensors = &dense_chain, 
)


AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 45484, 1,
                     NULL),
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 480, 1,
                     NULL),
  AI_TENSOR_LIST_IO_ENTRY(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &input_0_output),
  AI_TENSOR_LIST_IO_ENTRY(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &dense_output),
  &lstm_layer, 0, NULL)



AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_buffer* activation_buffer)
{
  AI_ASSERT(net_ctx &&  activation_buffer && activation_buffer->data)

  ai_ptr activations = AI_PTR(AI_PTR_ALIGN(activation_buffer->data, 4));
  AI_ASSERT(activations)
  AI_UNUSED(net_ctx)

  {
    /* Updating activations (byte) offsets */
    input_0_output_array.data = AI_PTR(NULL);
    input_0_output_array.data_start = AI_PTR(NULL);
    lstm_output_array.data = AI_PTR(activations + 0);
    lstm_output_array.data_start = AI_PTR(activations + 0);
    lstm_1_output_array.data = AI_PTR(activations + 360);
    lstm_1_output_array.data_start = AI_PTR(activations + 360);
    dense_output_array.data = AI_PTR(NULL);
    dense_output_array.data_start = AI_PTR(NULL);
    
  }
  return true;
}



AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network* net_ctx, const ai_buffer* weights_buffer)
{
  AI_ASSERT(net_ctx &&  weights_buffer && weights_buffer->data)

  ai_ptr weights = AI_PTR(weights_buffer->data);
  AI_ASSERT(weights)
  AI_UNUSED(net_ctx)

  {
    /* Updating weights (byte) offsets */
    
    dense_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_bias_array.data = AI_PTR(weights + 45480);
    dense_bias_array.data_start = AI_PTR(weights + 45480);
    dense_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_weights_array.data = AI_PTR(weights + 45360);
    dense_weights_array.data_start = AI_PTR(weights + 45360);
    lstm_1_bias_array.format |= AI_FMT_FLAG_CONST;
    lstm_1_bias_array.data = AI_PTR(weights + 44880);
    lstm_1_bias_array.data_start = AI_PTR(weights + 44880);
    lstm_1_peephole_array.format |= AI_FMT_FLAG_CONST;
    lstm_1_peephole_array.data = AI_PTR(weights + 44520);
    lstm_1_peephole_array.data_start = AI_PTR(weights + 44520);
    lstm_1_recurrent_array.format |= AI_FMT_FLAG_CONST;
    lstm_1_recurrent_array.data = AI_PTR(weights + 30120);
    lstm_1_recurrent_array.data_start = AI_PTR(weights + 30120);
    lstm_1_kernel_array.format |= AI_FMT_FLAG_CONST;
    lstm_1_kernel_array.data = AI_PTR(weights + 15720);
    lstm_1_kernel_array.data_start = AI_PTR(weights + 15720);
    lstm_bias_array.format |= AI_FMT_FLAG_CONST;
    lstm_bias_array.data = AI_PTR(weights + 15240);
    lstm_bias_array.data_start = AI_PTR(weights + 15240);
    lstm_peephole_array.format |= AI_FMT_FLAG_CONST;
    lstm_peephole_array.data = AI_PTR(weights + 14880);
    lstm_peephole_array.data_start = AI_PTR(weights + 14880);
    lstm_recurrent_array.format |= AI_FMT_FLAG_CONST;
    lstm_recurrent_array.data = AI_PTR(weights + 480);
    lstm_recurrent_array.data_start = AI_PTR(weights + 480);
    lstm_kernel_array.format |= AI_FMT_FLAG_CONST;
    lstm_kernel_array.data = AI_PTR(weights + 0);
    lstm_kernel_array.data_start = AI_PTR(weights + 0);
  }

  return true;
}


/**  PUBLIC APIs SECTION  *****************************************************/

AI_API_ENTRY
ai_bool ai_network_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if ( report && net_ctx )
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = {AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR,
                            AI_TOOLS_API_VERSION_MICRO, 0x0},

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 33690,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .activations       = AI_STRUCT_INIT,
      .params            = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if ( !ai_platform_api_get_network_report(network, &r) ) return false;

    *report = r;
    return true;
  }

  return false;
}

AI_API_ENTRY
ai_error ai_network_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}

AI_API_ENTRY
ai_error ai_network_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}

AI_API_ENTRY
ai_handle ai_network_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}

AI_API_ENTRY
ai_bool ai_network_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if ( !net_ctx ) return false;

  ai_bool ok = true;
  ok &= network_configure_weights(net_ctx, &params->params);
  ok &= network_configure_activations(net_ctx, &params->activations);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}

AI_API_ENTRY
ai_i32 ai_network_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}

#undef AI_NETWORK_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_VERSION_MAJOR
#undef AI_TOOLS_VERSION_MINOR
#undef AI_TOOLS_VERSION_MICRO
#undef AI_TOOLS_API_VERSION_MAJOR
#undef AI_TOOLS_API_VERSION_MINOR
#undef AI_TOOLS_API_VERSION_MICRO
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

