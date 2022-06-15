import 'dart:async';
import 'dart:convert';
import 'dart:io';
import 'package:flutter/material.dart';
import 'Models.dart';
import 'package:dio/dio.dart';
import "Pessoa.dart";

void main() => runApp(const MyApp());

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  static const String _title = 'Pessoas Cadastro';

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      title: _title,
      home: MyStateteHome(),
    );
  }
}

ListView _jobsListView(data) {

  return ListView.builder(
      itemCount: data.length,
      itemBuilder: (context, int index) {
        return _tile(
            context, data[index]['firstName'], 
            data[index]['surname'].toString(), 
            data[index]['age'],
            data[index]['id'],
        );
      });
}


ListTile _tile(BuildContext context, String firstname, String surname, int age, String id){
  var _age = age.toString();
  return ListTile(
      title: Text(firstname),
      subtitle: Text("tem $_age anos."),
      leading: const Icon(Icons.people),
      trailing: const Icon(Icons.edit),
      onTap: () {
        Navigator.push(
          context,
          MaterialPageRoute(
            builder: (context) =>
                DetailScreen(pessoa: Pessoa(firstname:firstname, surname:surname, age:age, id:id)),
          ),
        );
      },
    );
}


class MyStateteHome extends StatefulWidget {
  const MyStateteHome({Key? key}) : super(key: key);

  @override
  State<MyStateteHome> createState() => _MyStateHome();
}


Widget projectWidget() {
  late Dio dio;
  var data;

  Future getProfile() async {
    return Future.delayed(const Duration(seconds: 2), () async{
      BaseOptions options = BaseOptions(
        baseUrl: "http://localhost:8080",
        connectTimeout: 5000,
      );
      dio = Dio(options);
      Response response = await dio.get("/pessoa");
      var profile = response.data;
      data = profile;
      // DateTime date = DateTime.parse(data['creationData']);
      // print(date.toString());
      return profile;
    });
  }
  var loading = true;
  
  return FutureBuilder(
    builder: (context,AsyncSnapshot snapshot) {      
      if (snapshot.hasError) {
        loading = false;
        return const Center(child: CircularProgressIndicator());
      }

      if (snapshot.connectionState == ConnectionState.none &&
          snapshot.hasData == null) {
        print('project snapshot data is: ${snapshot.data}');
        return const Center(
          child: CircularProgressIndicator(),
         );
      }

      if (loading){
        loading = false;
        return const Center(child: CircularProgressIndicator());
      }
      loading = false;
      return ListView.builder(
        scrollDirection: Axis.vertical,
        itemExtent: 80,
        itemCount: snapshot.data!.length,
        itemBuilder: (context, index) {
          return _tile(
            context, data[index]['firstName'], 
            data[index]['surname'].toString(), 
            data[index]['age'],
            data[index]['id'],
        );
        }
      );
    },
    future: getProfile(),
  );
}

class _MyStateHome extends State<MyStateteHome> {
 
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Pessoas Cadastro'),
      ),
      body: projectWidget(),
      
      floatingActionButtonLocation: FloatingActionButtonLocation.centerDocked,
      floatingActionButton: FloatingActionButton(
        tooltip: 'Novo Cadastro',
        backgroundColor: Colors.deepPurpleAccent,
        elevation: 2.0,
        onPressed: () {
          Navigator.push(
            context,
            MaterialPageRoute(
              builder: (context) => const AddData(
                  /*pessoa: Pessoa("Marcos", "Fernandes", 7)*/
                  ),
            ),
          );
        },
        child: const Icon(Icons.add),
      ),
    );
  }
}

class DetailScreen extends StatelessWidget {
  const DetailScreen({required this.pessoa});

  final Pessoa pessoa;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Olá ${pessoa.firstname}"),
      ),
      floatingActionButton: Wrap(
          //will break to another line on overflow
          direction: Axis.horizontal, //use vertical to show  on vertical axis
          children: <Widget>[
            Container(
                margin: const EdgeInsets.all(10),
                child: FloatingActionButton(
                  tooltip: "Editar Dados",
                  heroTag: 'edit-data',
                  onPressed: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context) => EditData(pessoa),
                      ),
                    );
                  },
                  backgroundColor: Colors.deepPurpleAccent,
                  child: const Icon(Icons.edit),
                )),
            Container(
                margin: const EdgeInsets.all(10),
                child: FloatingActionButton(
                  tooltip: "Deletar Dados",
                  heroTag: 'delete-data',
                  onPressed: () {
                    DeleteData().delete(pessoa.id);    
                    Navigator.pushAndRemoveUntil(
                      context,
                      MaterialPageRoute(builder: (context) => const MyStateteHome()), // this mymainpage is your page to refresh
                      (Route<dynamic> route) => false,
                    );
                  },
                  child: const Icon(Icons.delete)
                )),
          ]),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child:
            Text("Olá ${pessoa.firstname} ${pessoa.surname}\nIdade: ${pessoa.age}\nID ${pessoa.id}"),
      ),
    );
  }
}

class EditData extends StatelessWidget {
  EditData(this.pessoa, {Key? key}) : super(key: key);
  PessoaBody pessoabody = PessoaBody();
  final Pessoa pessoa;
  final _formKey = GlobalKey<FormState>();
  

  void upateProfile(String id, PessoaBody pessoaBody) async {
    BaseOptions options =  BaseOptions(
      baseUrl: "http://localhost:8080",
      connectTimeout: 5000,
    );
  
    Dio dio = Dio(options);
   
    Response response = await dio.put("/pessoa/$id", data:pessoaBody.toJson(), options: Options(headers: {
      HttpHeaders.contentTypeHeader: "application/json"})
    );
    
    print(response.data);   
  }


  @override
  Widget build(BuildContext context) {
    
    return Scaffold(
        appBar: AppBar(
          title: const Text("Editar Dados"),
        ),
        body: Form(
          key: _formKey,
          child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: <Widget>[
            Padding(
                padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 16),
                child: TextFormField(
                  initialValue: pessoa.firstname,
                  decoration: const InputDecoration(
                    border: OutlineInputBorder(),
                    labelText: 'Informe seu novo Nome',
                  ),
                  // The validator receives the text that the user has entered.
                  validator: (value) {
                    if (value == null || value.isEmpty) {
                      return 'Informe seu Nome!';
                    }
                    pessoabody.firstname = value;
                    return null;
                  },
                )),
            Padding(
                padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 16),
                child: TextFormField(
                  initialValue: pessoa.surname,
                  decoration: const InputDecoration(
                    border: OutlineInputBorder(),
                    labelText: 'Informe seu novo SobreNome',
                  ),
                  // The validator receives the text that the user has entered.
                  validator: (value) {
                    if (value == null || value.isEmpty) {
                      return 'Informe seu SobreNome!';
                    }

                    pessoabody.surname = value;

                    return null;
                  },
                )),
            Padding(
                padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 16),
                child: TextFormField(
                  initialValue: pessoa.age.toString(),
                  keyboardType: TextInputType.number,
                  decoration: const InputDecoration(
                    border: OutlineInputBorder(),
                    labelText: 'Informe sua nova Idade',
                  ),
                  // The validator receives the text that the user has entered.
                  validator: (value) {
                    if (value == null || value.isEmpty) {
                      return 'Por favor digite sua idade, use apenas números';
                    }
                    
                    try {
                      pessoabody.age = int.parse(value).toInt();
                      
                    } catch (FormatException) {
                      return 'Por favor digite sua idade, utilize apenas números';
                    }

                    return null;
                  },
                )),
            Padding(
              padding: const EdgeInsets.symmetric(vertical: 16.0),
              child: ElevatedButton(
                onPressed: () {
                  // Validate returns true if the form is valid, or false otherwise.
                  if (_formKey.currentState!.validate()) {
                    // If the form is valid, display a snackbar. In the real world,
                    // you'd often call a server or save the information in a database.
                    upateProfile(pessoa.id, pessoabody);
                    ScaffoldMessenger.of(context).showSnackBar(
                      const SnackBar(content: Text('Os dados foram atualizado!')),
                    );
                    Navigator.pushAndRemoveUntil(
                      context,
                      MaterialPageRoute(builder: (context) => const MyStateteHome()), // this mymainpage is your page to refresh
                      (Route<dynamic> route) => false,
                  );
                  }
                },
                child: const Text('Atualizar'),
              ),
            ),
          ],
          ),
        )
      );
  }
}

class DeleteData  {

  void delete(String id) async {
    BaseOptions options =  BaseOptions(
      baseUrl: "http://localhost:8080",
      connectTimeout: 5000,
    );
  
    Dio dio = new Dio(options);
    Response response = await dio.delete("/pessoa/$id");
    print(response.data);   
  }
}


class AddData extends StatelessWidget {
  const AddData({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: const Text("Novo Cadastro"),
        ),
        body: const AddCustomFormState());
  }
}

// Create a Form widget: adicionar novo cadastro.
class AddCustomFormState extends StatefulWidget {
  const AddCustomFormState({Key? key}) : super(key: key);

  @override
  // ignore: library_private_types_in_public_api
  _AddCustomFormState createState() {
    return _AddCustomFormState();
  }
}

// Create a corresponding State class.
// This class holds data related to the form.
class _AddCustomFormState extends State<AddCustomFormState> {
  // Create a global key that uniquely identifies the Form widget
  // and allows validation of the form.
  //
  // Note: This is a GlobalKey<FormState>,
  // not a GlobalKey<MyCustomFormState>.
  final _formKey = GlobalKey<FormState>();
  late Dio _dio;
  PessoaBody pessoabody = PessoaBody();

  @override
  void initState() {
    super.initState();

    BaseOptions options =  BaseOptions(
      baseUrl: "http://localhost:8080",
      connectTimeout: 5000,
    );

    _dio = Dio(options);
  }
  void postProfile(PessoaBody pessoabody) async{
    var data = pessoabody.toJson();
    _dio.options.contentType= Headers.formUrlEncodedContentType;

    await _dio.post("/pessoa",  options: Options(headers: {
    HttpHeaders.contentTypeHeader: "application/json",
    }),
  
    data: jsonEncode(data),);
  }

  @override
  Widget build(BuildContext context) {
    // Build a Form widget using the _formKey created above.
    
    return Form(
      key: _formKey,
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: <Widget>[
          Padding(
              padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 16),
              child: TextFormField(
                decoration: const InputDecoration(
                  border: OutlineInputBorder(),
                  labelText: 'Informe seu Nome',
                ),
                // The validator receives the text that the user has entered.
                validator: (value) {
                  if (value == null || value.isEmpty) {
                    return 'O campo não pode ser vazio!';
                  }

                  pessoabody.firstname = value;
                  return null;
                },
              )),
          Padding(
              padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 16),
              child: TextFormField(
                decoration: const InputDecoration(
                  border: OutlineInputBorder(),
                  labelText: 'Informe seu SobreNome',
                ),
                // The validator receives the text that the user has entered.
                validator: (value) {
                  if (value == null || value.isEmpty) {
                    return 'O campo não pode ser vazio!';
                  }

                  pessoabody.surname = value;
                  
                  return null;
                },
              )),
          Padding(
              padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 16),
              child: TextFormField(
                keyboardType: TextInputType.number,
                decoration: const InputDecoration(
                  border: OutlineInputBorder(),
                  labelText: 'Informe sua Idade',
                  
                ),
                // The validator receives the text that the user has entered.
                validator: (value) {
                  if (value == null || value.isEmpty) {
                    return 'Por favor informe apenas númeo(s)';
                  }
                  
                  
                  try {
                    pessoabody.age = int.parse(value).toInt();
                      
                  } catch (FormatException) {
                    return 'Por favor digite sua idade, utilize apenas números';
                  }

                  return null;
                },
              )),
          Padding(
            padding: const EdgeInsets.symmetric(vertical: 16.0),
            child: ElevatedButton(
              onPressed: () {
                // Validate returns true if the form is valid, or false otherwise.
                
                if (_formKey.currentState!.validate()) {
                  // If the form is valid, display a snackbar. In the real world,
                  // you'd often call a server or save the information in a database.
                  ScaffoldMessenger.of(context).showSnackBar(    
                    const SnackBar(content: Text('Novo Cadastro Realizado')),
                  );
                  
                                   
                  postProfile(pessoabody);

                  Navigator.pushAndRemoveUntil(
                      context,
                      MaterialPageRoute(builder: (context) => const MyStateteHome()), // this mymainpage is your page to refresh
                      (Route<dynamic> route) => false,
                  );

                  // Navigator.pop(context,true);  
                }
              },
              child: const Text('Adicionar'),
            ),
          ),
        ],
      ),
    );
  }
}
