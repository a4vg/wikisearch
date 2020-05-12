# cs3102-TrabajoFinal

# Trabajo Final
#utec/CS3102

## Objetivo

> **Se pide hacer un motor de busqueda al estilo de google.**   


## Hitos: 
**Parcial**  16 - Jun
- Descargar la base de datos de wikipedia, sin imagenes ([Wikipedia:Descargas - Wikipedia, la enciclopedia libre](https://es.wikipedia.org/wiki/Wikipedia:Descargas))
- Preprocesar data  (eliminar stopswords, lematizar, tokenizar) 
- Preparar descriptor (i.e. BoW, CBoW - words2vect) para resultados similares dentro de los documentos.
- Indexar data (bTree+)
- Grabar y Cargar arbol  a demanda.

**Final**  21 - Jul
- Crear servicio web para consultas
    - Traversal debe realizarse recorriendo el arbol usando threads (multiples consultas a la vez, multiples usuarios haciendo consultas 1 solo arbol recorriendose)
    - La busqueda puede realizarse usando varias palabras (i.e viaje de campamento)
    - Mostrar los 10 primeros resultados mas relevantes a la consulta y el tiempo de consulta.  (crear Crear Page-Rank si es necesario) (titulo + texto descriptivo 100  + palabras claves) 
    - Autocorrector de palabras (Trie, Patricia)

**Extra**
- Mostrar publicidad  (Crear Crawler y Scrapper para retornar paginas similares )
		

