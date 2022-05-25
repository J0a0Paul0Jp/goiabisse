using WebApi.Models;
using Microsoft.EntityFrameworkCore;

namespace WebApi.Service;

public class DataContext : DbContext
{
    public DbSet<Pessoa> ?Pessoas { get; set; }
    protected  IConfiguration Configuration;

    public DataContext(DbContextOptions<DataContext> options) : base(options)
    {
        
       
    }

    protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
    {
        var configuration = new ConfigurationBuilder()
                .SetBasePath(Directory.GetCurrentDirectory())
                .AddJsonFile("appsettings.json")
                .Build();

        var connectionString = configuration.GetConnectionString("WebApiDatabase");
        optionsBuilder.UseSqlServer(connectionString);
    }

  
}