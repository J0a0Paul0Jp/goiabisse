// using System;
// using System.Collections.Generic;
// using Moq;
// using WebApi.Controller;
// using WebApi.Context;
// namespace WebApi.Tests;

using WebApi.Models;

namespace WebApi.Data;

public static class PessoaData
{
    static List<Pessoa> Pessoas { get; }
    static String nextId = "3";
    static PessoaData()
    {
        Pessoas = new List<Pessoa>
        {
            new Pessoa { Id = "1", FirstName = "Alice", Age = 23 },
            new Pessoa { Id = "2", FirstName = "João",  Age = 20 }
        };
    }

    public static List<Pessoa> GetAll() => Pessoas;

    public static Pessoa? Get(String id) => Pessoas.FirstOrDefault(p => p.Id==id);

    public static void Add(Pessoa pessoa)
    {
        pessoa.Id = nextId;
        Pessoas.Add(pessoa);
    }

    public static void Delete(String id)
    {
        var pessoa = Get(id);
        if(pessoa is null)
            return;

        Pessoas.Remove(pessoa);
    }

    public static void Update(Pessoa pessoa)
    {
        var index = Pessoas.FindIndex(p => p.Id == pessoa.Id);
        if(index == -1)
            return;

        Pessoas[index] = pessoa;
    }
}