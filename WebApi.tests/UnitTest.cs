using WebApi.Models;
using Moq;
using Xunit;
using WebApi.Service;
using WebApi.Controllers;
using Microsoft.AspNetCore.Mvc; 
using Microsoft.Extensions.Logging;
using Microsoft.EntityFrameworkCore;

namespace WebApi.Controllers.Test;

public class TodoMockData
{
    public static List<Pessoa> GetAll()
    {
        return new List<Pessoa>{
             new Pessoa{
                 Age = 1,
                 FirstName = "João",
                 
             },
             new Pessoa{
                 Age = 2,
                 FirstName = "Maria",
             },
             new Pessoa{
                 Age = 3,
                 FirstName = "Eduardo",
             }
         };
    }

    public static Pessoa GetId(String id)
    {
        if (id == "12345")
        {
            return new Pessoa
            {
                    Age = 20,
                    FirstName = "João",
            };
        }

        return null;
    }
    
    public static bool Create(Pessoa pessoa)
    {
        return true;
    }


    public static Pessoa GetObjectResultContent<T>(ActionResult<T> result)
    {
        return (Pessoa) ((ObjectResult) result.Result).Value;
    }

    public static bool Delete(String id) {
        return id == "12345";
    }
}

public class TestController
{
    [Fact]
    public  void TestGetAllController()
    {
        /// Arrange
        var todoService = new Mock<ITodoService>();
        // var logger = new Mock.Of<ILogger<PesssoaControler>>();
        var logger = new Mock<ILogger<PessoaController>>();
        // var logger = Mock.Of<ILogger<BlogController>>()

        
        todoService.Setup(_ => _.GetAll()).Returns(TodoMockData.GetAll());

        var sut = new PessoaController(todoService.Object, logger.Object);

        /// Act
        var result = (OkObjectResult) sut.GetAll();


        // /// Assert
        // result.StatusCode.Should().Be(200);
        Assert.NotNull(result);
        Assert.Equal(200, result.StatusCode);
        // Assert.Equal(404, result.StatusCode);
    }

    [Fact]
    public  void TestGetIdController()
    {
        /// Arrange
        var todoService = new Mock<ITodoService>();
        var logger = new Mock<ILogger<PessoaController>>();

        
        todoService.Setup(_ => _.GetId("12345")).Returns(TodoMockData.GetId("12345"));
        
        var sut = new PessoaController(todoService.Object, logger.Object);

        /// Act
        var result = sut.Get("12345").Result as OkObjectResult;
        var resultObject = TodoMockData.GetObjectResultContent<Pessoa>(result);
        Assert.Equal("João", resultObject.FirstName);
        // Assert.Equal(10, resultObject.Age);
        Assert.Equal(20, resultObject.Age);
    }

    [Fact]
    public  void TestCreateController()
    {
        /// Arrange
        var todoService = new Mock<ITodoService>();
        var logger = new Mock<ILogger<PessoaController>>();
        var pessoa = new Pessoa
        {
            Age = 4,
            FirstName = "Kelly",
        };
        
        todoService.Setup(_ => _.Create(pessoa)).Returns(TodoMockData.Create(pessoa));
        
        var sut = new PessoaController(todoService.Object, logger.Object);

        /// Act
        var action =  sut.Create(pessoa);
        OkObjectResult result = action as OkObjectResult;
        Assert.Equal("Adicionado", result.Value);
    }

    [Fact]
    public void TestDeleteController()
    {
        var todoService = new Mock<ITodoService>();
        var logger = new Mock<ILogger<PessoaController>>();
        todoService.Setup(_ => _.Delete("12345")).Returns(TodoMockData.Delete("12345"));
        var sut = new PessoaController(todoService.Object, logger.Object);

        var action =  sut.Delete("12345");
        OkObjectResult result = action as OkObjectResult;
        Assert.Equal("Removido", result.Value);
    }
}
