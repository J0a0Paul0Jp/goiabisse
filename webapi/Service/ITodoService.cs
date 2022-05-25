using WebApi.Models;
namespace WebApi.Service;

public interface ITodoService
{
    List<Pessoa>GetAll();
    Pessoa GetId(string id);
    
    bool SaveChanges();
    bool Update(Pessoa id);
    bool Delete(String id);
    bool Create(Pessoa pessoa);
}
