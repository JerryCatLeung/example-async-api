
#ifndef MyDTOs_hpp
#define MyDTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class HelloDto : public oatpp::DTO {
  
  DTO_INIT(HelloDto, DTO)
  
  DTO_FIELD(String, userAgent, "user-agent");
  DTO_FIELD(String, message);
  DTO_FIELD(String, server);
  
};

class MessageDto : public oatpp::DTO {
  
  DTO_INIT(MessageDto, DTO)
  
  DTO_FIELD(String, message);
  DTO_FIELD(String, name);
  DTO_FIELD(String, version);
  DTO_FIELD(String, tag);
  DTO_FIELD(Int32, age);
  DTO_FIELD(Vector<Vector<Int32>>, featureIds);
  DTO_FIELD(Vector<Vector<Float32>>, featureValues);
  DTO_FIELD(Float32, predict);
  
};

class ResponseDto : public oatpp::DTO{

  DTO_INIT(ResponseDto, DTO);

  DTO_FIELD(String, name);
  DTO_FIELD(Vector<Float32>, predicts);
  
};

#include OATPP_CODEGEN_END(DTO)

#endif /* MyDTOs_hpp */
