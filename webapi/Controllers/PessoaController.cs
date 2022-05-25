using WebApi.Models;
using Microsoft.AspNetCore.Mvc; 
using WebApi.Service;

namespace WebApi.Controllers;

[ApiController]
[Route("[controller]")]
public class PessoaController : ControllerBase
{
    private readonly ITodoService _todoService;
    private readonly ILogger<PessoaController> _logger;
    
    public PessoaController(ITodoService service, ILogger<PessoaController> logger)
    {
        _todoService = service;
        _logger = logger;
    }

    //GET /Users/
    [HttpGet]
    public IActionResult GetAll()
    {
       _logger.LogInformation("Nova requisição GET ALL na API");
        return Ok(
                 _todoService.GetAll()
        );
       return Ok("ok");
    }

    // GET /Users
    [HttpGet("{id}")]
    public ActionResult<Pessoa> Get(String id)
    {
        var pessoa = _todoService.GetId(id);

        if(pessoa == null) {
            _logger.LogWarning("Get({id}) NÃO ENCONTRADO", id);
            return NotFound();
        }

        _logger.LogInformation("Foi requisitado o id {0}, no método Get, obtendo sucesso.", id);

        return Ok(pessoa);  
    }

    //Post /Users/
    [HttpPost]
    public IActionResult Create(Pessoa pessoa)
    {
        pessoa.CreationDate = DateTime.Now;

        
        if (_todoService.Create(pessoa))
        {
            _logger.LogInformation("Foi inserido um novo usuário de id ({0}), na base de dados", pessoa.Id);
        
            return Ok("Adicionado");
        }

        return Ok("Erro ao tentar adicioar");
    }

    //PUT /Users/{id}
    [HttpPut("{id}")]
    public IActionResult Update(String id, Pessoa pessoa)
    {
        Pessoa aux = _todoService.GetId(id);

        if (aux!=null)
        {
            if (pessoa.FirstName!=null)   
                aux.FirstName = pessoa.FirstName;
            if (pessoa.Surname!=null)
                aux.Surname = pessoa.Surname;
            if (pessoa.Age!=null)
                aux.Age = pessoa.Age;

            aux.Id = aux.Id;
            _todoService.SaveChanges();
            _logger.LogInformation("O dados referente ao id({0}), foi atualizado base de dados", aux.Id);
        
            return Ok("Atualizado");
        }

        _logger.LogWarning("Put({id}) informações não se correspodem", id);
        return BadRequest();
    }

    //DELETE /Users/{id}
    [HttpDelete("{id}")]
    public IActionResult Delete(String id)
    {   
        if (_todoService.Delete(id))
        {
            _logger.LogInformation("O dados referente ao id({0}) foram excluídos", id);
            return Ok("Removido");
        }
        
        return NotFound();
        
    }
}