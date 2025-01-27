
#ifndef MyController_hpp
#define MyController_hpp

#include "dto/MyDTOs.hpp"
#include "predictor/OnnxRuntimePredictor.hpp"
#include <iterator>

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include <iostream>
#include <chrono>


#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin codegen

/**
 *  ApiController
 *  Creating ENDPOINTs
 */
class MyController : public oatpp::web::server::api::ApiController {
private:
  // 添加OnnxRuntimePredictor的成员变量
  static const char* model_path;
  static OnnxRuntimePredictor* m_predictor;
protected:
  MyController(const std::shared_ptr<ObjectMapper>& objectMapper)
  : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:
  
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<MyController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                    objectMapper)){
    return std::shared_ptr<MyController>(new MyController(objectMapper));
  }
  
  /**
   *  Hello World endpoint Coroutine mapped to the "/" (root)
   */
  ENDPOINT_ASYNC("GET", "/", Root) {
    
    ENDPOINT_ASYNC_INIT(Root)
    
    /**
     *  Coroutine entrypoint act()
     *  returns Action (what to do next)
     */
    Action act() override {
      auto dto = HelloDto::createShared();
      dto->message = "Hello Async!";
      dto->server = Header::Value::SERVER;
      dto->userAgent = request->getHeader(Header::USER_AGENT);
      return _return(controller->createDtoResponse(Status::CODE_200, dto));
    }
    
  };
  
  /**
   *  Echo body endpoint Coroutine. Mapped to "/body/string".
   *  Returns body received in the request
   */
  ENDPOINT_ASYNC("GET", "/body/string", EchoStringBody) {
    
    ENDPOINT_ASYNC_INIT(EchoStringBody)
    
    Action act() override {
      /* return Action to start child coroutine to read body */
      return request->readBodyToStringAsync().callbackTo(&EchoStringBody::returnResponse);
    }
    
    Action returnResponse(const oatpp::String& body){
      /* return Action to return created OutgoingResponse */
      return _return(controller->createResponse(Status::CODE_200, body));
    }
    
  };
  
  /**
   *  Echo body endpoint Coroutine. Mapped to "/body/dto".
   *  Deserialize DTO reveived, and return same DTO
   *  Returns body as MessageDto received in the request
   */
  ENDPOINT_ASYNC("POST", "/body/dto", EchoDtoBody) {
    
    ENDPOINT_ASYNC_INIT(EchoDtoBody)
    
    Action act() override {
      return request->readBodyToDtoAsync<oatpp::Object<MessageDto>>(controller->getDefaultObjectMapper()).callbackTo(&EchoDtoBody::returnResponse);;
    }
    
    Action returnResponse(const oatpp::Object<MessageDto>& body){
      auto responseDto = ResponseDto::createShared();
      responseDto->req_id = body->req_id;
      std::vector<int64_t> featIdsTensorValuesStd;
      for(auto it = body->featIdsTensor->begin(); it != body->featIdsTensor->end(); ++it) {
        auto element = *it;
        featIdsTensorValuesStd.push_back(static_cast<int64_t>(element));
      }
      std::vector<float> featValuesTensorValuesStd;
      for(auto it = body->featValsTensor->begin(); it != body->featValsTensor->end(); ++it) {
        auto element = *it;
        featValuesTensorValuesStd.push_back(static_cast<float>(element));
      }
      // In the function where you call the predictor
      std::vector<float> predictResult = m_predictor->predictor(featIdsTensorValuesStd, featValuesTensorValuesStd);

      responseDto->predicts = oatpp::Vector<Float32>::createShared();
      for(auto &val : predictResult) {
        responseDto->predicts->push_back(val);
      }

      return _return(controller->createDtoResponse(Status::CODE_200, responseDto));
    }
    
  };
  
};

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- End codegen

#endif /* MyController_hpp */
