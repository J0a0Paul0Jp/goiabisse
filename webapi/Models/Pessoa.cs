using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;

namespace WebApi.Models;

//USER
public class Pessoa
{
    
    [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
    public String ?Id{get; set;}
    
    [Required]
    public String FirstName {get; set;}
    
    public String ?Surname {get; set;}
    
    [Required]
    public int Age{get; set;}
    
    public DateTime? CreationDate{get; set;}
}
