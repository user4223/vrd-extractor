
#include "vrdlib/common/include/CPropertySourceFactory.h"

#include "../include/CSampleDirectoryAwareTestBase.h"
#include "../include/CConflictHandlerMock.h"

#include <gtest/gtest.h>

#include <memory>

struct PropertySourceFactoryTest : public VRD::Test::CSampleAwareTestBase
{
   virtual void SetUp() override
   {
      CSampleAwareTestBase::SetUp();
      m_handlerFactory = std::make_unique<VRD::Test::CConflictHandlerMockFactory>([]
                                                                                  { return std::make_unique<VRD::Test::CConflictHandlerMock>(); });
   }

   virtual void TearDown() override
   {
      CSampleAwareTestBase::TearDown();
      m_handlerFactory.reset();
   }

   VRD::API::IConflictHandlerFactory &getConflictHandlerFactory() { return *m_handlerFactory; }

private:
   std::unique_ptr<VRD::API::IConflictHandlerFactory> m_handlerFactory;
};

TEST_F(PropertySourceFactoryTest, UnknownType)
{
   VRD::Common::CPropertySourceFactory factory(getConflictHandlerFactory());
   EXPECT_THROW(factory.create(getSampleDirectory() / "blub.blub"), std::invalid_argument);
}

TEST_F(PropertySourceFactoryTest, VRDType)
{
   VRD::Common::CPropertySourceFactory factory(getConflictHandlerFactory());
   std::unique_ptr<VRD::API::IPropertySource> source;
   EXPECT_NO_THROW(source = factory.create(getSampleDirectory() / "VRD" / "V3_CM4.vrd"));
}

TEST_F(PropertySourceFactoryTest, DR4Type)
{
   VRD::Common::CPropertySourceFactory factory(getConflictHandlerFactory());
   std::unique_ptr<VRD::API::IPropertySource> source;
   EXPECT_NO_THROW(source = factory.create(getSampleDirectory() / "DR4" / "V4_CM2.dr4"));
}
