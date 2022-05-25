using WebApi.Models;
namespace WebApi.Service;

public class TodoService : ITodoService
{
    private  DataContext _context;
    
    public TodoService(DataContext context)
    {
        _context = context;
    }

    public List<Pessoa> GetAll()
    {
        return  _context.Pessoas.ToList();
    }

    public  bool SaveChanges()
    {
        var resultado = _context.SaveChanges();
        return resultado!=null;
    }

    public Pessoa GetId(string id)
    {
        var pessoa = _context.Pessoas.FirstOrDefault(
            x => x.Id == id
        );

        return pessoa;
    }

    public bool Create(Pessoa pessoa){
        var resultado = _context.Add(pessoa);
        _context.SaveChanges();
        return resultado != null;       
    }

    public bool Delete(string id)
    {
        var pessoa = _context.Pessoas.FirstOrDefault(
            x => x.Id == id
        );
        if (pessoa == null)
            return false;

        var resultado = _context.Pessoas.Remove(pessoa);
        _context.SaveChanges();
        return  true;
    }

    public bool Update(Pessoa pessoa)
    {
        var resultado = _context.Pessoas.Update(pessoa);           
        _context.SaveChanges();
        return resultado != null;
    }
}