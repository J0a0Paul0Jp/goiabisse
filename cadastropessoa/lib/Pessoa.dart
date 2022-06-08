class PessoaBody{
  late String firstName;
  late String surname;
  late int age;

  set firstname(String firstname) {
    firstName = firstname;
  }
  
  Pessoa(){
    
  }

  Map<String, dynamic>toJson(){
    return {
      "firstname":firstName,
      "surname":surname,
      "age":age
    };
  }
}