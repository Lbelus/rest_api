#include <gtest/gtest.h>
#include "example_repository.hpp"

//ExampleUsersRepositoryImpl 
TEST(FakeRepo, CreateAssignsIdAndReturnsSuccess)
{
    ExampleUsersRepositoryImpl concrete;
    IExampleUsersRepository& repo = concrete;
    ExampleUser u = FakeExampleUsersRepository::make_user("Ada", "ada@example.com");

    EXPECT_EQ(repo.create(u), EXIT_SUCCESS);
    ExampleUser created = repo.get_mapped_entry();
    EXPECT_GT(int(created.id), 0);
    EXPECT_EQ(std::string(created.name), "Ada");
    EXPECT_EQ(std::string(created.email), "ada@example.com");
}

TEST(FakeRepo, DuplicateEmailFails)
{
    ExampleUsersRepositoryImpl concrete;
    IExampleUsersRepository& repo = concrete;
    EXPECT_EQ(repo.create(FakeExampleUsersRepository::make_user("A", "e@x.com")), EXIT_SUCCESS);
    EXPECT_EQ(repo.create(FakeExampleUsersRepository::make_user("B", "e@x.com")), EXIT_FAILURE);
    EXPECT_STREQ(repo.error(), "duplicate email");
}

TEST(FakeRepo, GetByIdSuccessAndFailure)
{
    ExampleUsersRepositoryImpl concrete;
    IExampleUsersRepository& repo = concrete;
    repo.create(FakeExampleUsersRepository::make_user("A", "a@x.com"));
    int id = repo.get_mapped_entry().id;

    EXPECT_EQ(repo.get_by_id(id), EXIT_SUCCESS);
    EXPECT_EQ(std::string(repo.get_mapped_entry().name), "A");

    EXPECT_EQ(repo.get_by_id(9999), EXIT_FAILURE);
    EXPECT_STREQ(repo.error(), "not found");
}

TEST(FakeRepo, UpdateHappyPath)
{
    ExampleUsersRepositoryImpl concrete;
    IExampleUsersRepository& repo = concrete;
    repo.create(FakeExampleUsersRepository::make_user("A", "a@x.com"));
    ExampleUser u = repo.get_mapped_entry();
    u.name = "Ada Lovelace";
    EXPECT_EQ(repo.update(u), EXIT_SUCCESS);

    EXPECT_EQ(repo.get_by_id(u.id), EXIT_SUCCESS);
    EXPECT_EQ(std::string(repo.get_mapped_entry().name), "Ada Lovelace");
}

TEST(FakeRepo, UpdateNotFound)
{
    ExampleUsersRepositoryImpl concrete;
    IExampleUsersRepository& repo = concrete;
    ExampleUser ghost = FakeExampleUsersRepository::make_user("X", "x@x", 42);
    EXPECT_EQ(repo.update(ghost), EXIT_FAILURE);
    EXPECT_STREQ(repo.error(), "not found");
}

TEST(FakeRepo, RemoveWorksAndThenGetFails)
{
    ExampleUsersRepositoryImpl concrete;
    IExampleUsersRepository& repo = concrete;
    repo.create(FakeExampleUsersRepository::make_user("A", "a@x.com"));
    int id = repo.get_mapped_entry().id;

    EXPECT_EQ(repo.remove(id), EXIT_SUCCESS);
    EXPECT_EQ(repo.get_by_id(id), EXIT_FAILURE);
}

TEST(FakeRepo, ListAllOrdersDescAndPaginates)
{
    ExampleUsersRepositoryImpl concrete;
    IExampleUsersRepository& repo = concrete;
    repo.create(FakeExampleUsersRepository::make_user("A", "a@x.com"));
    int id1 = repo.get_mapped_entry().id;
    repo.create(FakeExampleUsersRepository::make_user("B", "b@x.com"));
    int id2 = repo.get_mapped_entry().id;
    repo.create(FakeExampleUsersRepository::make_user("C", "c@x.com"));
    int id3 = repo.get_mapped_entry().id;

    // limit 2, offset 0 -> highest ids first
    EXPECT_EQ(repo.list_all(2, 0), EXIT_SUCCESS);
    auto v = repo.get_mapped_entry_vector();
    ASSERT_EQ(v.size(), 2u);
    EXPECT_EQ(int(v[0].id), id3);
    EXPECT_EQ(int(v[1].id), id2);

    // next page
    EXPECT_EQ(repo.list_all(2, 2), EXIT_SUCCESS);
    v = repo.get_mapped_entry_vector();
    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(int(v[0].id), id1);
}
