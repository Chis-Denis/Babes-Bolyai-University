package repository;

import domain.Student;
import domain.validators.Validator;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

public class StudentXMLFileRepository extends InMemoryRepository<Long, Student> {
    private String fileName;

    public StudentXMLFileRepository(Validator<Student> validator, String fileName) {
        super(validator);
        this.fileName = fileName;
        loadData();
    }

    private void loadData() {
        try {
            Document document = DocumentBuilderFactory
                    .newInstance()
                    .newDocumentBuilder()
                    .parse(this.fileName);

            Element root = document.getDocumentElement();
            NodeList children = root.getChildNodes();
            for (int i = 0; i < children.getLength(); i++) {
                Node studentElement = children.item(i);
                if (studentElement instanceof Element) {
                    Student student = createStudentFromElement((Element) studentElement);
                    super.save(student);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void writeToFile() {
        try {
            Document document = DocumentBuilderFactory
                    .newInstance()
                    .newDocumentBuilder()
                    .newDocument();
            Element root = document.createElement("students");
            document.appendChild(root);
            super.findAll().forEach(s -> {
                Element e = createElementfromStudent(document, s);
                root.appendChild(e);
            });

            //write Document to file
            Transformer transformer = TransformerFactory.
                    newInstance().newTransformer();

            Source source=new DOMSource(document);

            transformer.transform(source,
                    new StreamResult(fileName));

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private Element createElementfromStudent(Document document, Student student) {
        Element element = document.createElement("student");

        element.setAttribute("id", student.getId().toString());

        Element name = document.createElement("nume");
        name.setTextContent(student.getName());
        element.appendChild(name);


        Element grupa = document.createElement("grupa");
        grupa.setTextContent(((Integer) student.getGroup()).toString());
        element.appendChild(grupa);

        return element;
    }

    @Override
    public Student save(Student entity) {
        Student student = super.save(entity);
        if (student == null) {
            writeToFile();
        }
        return student;
    }

    private Student createStudentFromElement(Element studentElement) {
        String id = studentElement.getAttribute("id");
        //NodeList nods = studentElement.getChildNodes();
        String nume = studentElement.getElementsByTagName("nume")
                .item(0)
                .getTextContent();

        String grupa = studentElement.getElementsByTagName("grupa")
                .item(0)
                .getTextContent();

        Student s= new Student( nume, Integer.parseInt(grupa));
        s.setId(Long.parseLong(id));
        return s;
    }
}
