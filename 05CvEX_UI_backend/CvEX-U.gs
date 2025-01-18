// Change for correct data from Sheets
var ss = SpreadsheetApp.openById('***********************');
var sheet = ss.getSheetByName('********');

function doGet(e) {
  var dateTime = new Date();
  var date = Utilities.formatDate(dateTime, Session.getScriptTimeZone(), "MM/dd/yyyy");
  var time = Utilities.formatDate(dateTime, Session.getScriptTimeZone(), "HH:mm:ss");

  // Data ESP32 will need to Send
  var rfid_code = e.parameters.rfid_code; // RFID Code in Hex format
  var remarks = Number(e.parameter.remarks); // 0 In, 1 Out

 // var rfid_code = "9B89DD13";
  //var remarks = 0;
  
  // Debugging logs
  Logger.log('rfid_code: ' + rfid_code);
  Logger.log('remarks: ' + remarks);

  var name;
  var identifier;
  var id;
  var vehicle_type;
  var vehicle_unit;
  var plate_number;
  var owner_link;
  var department_link;
  var count;

  // Registered codes
  if (rfid_code == "9B89DD13") {
    name = "Alexis Chelsy Julian";
    identifier = "Faculty";
    id = "202164782";
    vehicle_type = "Four Wheels";
    vehicle_unit = "Red Toyota Vios";
    plate_number = "ACJ 08103";
    owner_link = "https://raw.githubusercontent.com/Ze-r000/CvEX-U_Images/refs/heads/main/Faculty.png";
    department_link = "College of Engineering and Information Technology";
  } else if (rfid_code == "53F613DA") {
    name = "Joshua Joy Guasis";
    identifier = "Student";
    id = "202223490";
    vehicle_type = "Motor Cycle";
    vehicle_unit = "Black Honda ADV";
    plate_number = "JJG 47032";
    owner_link = "https://raw.githubusercontent.com/Ze-r000/CvEX-U_Images/refs/heads/main/Student.png";
    department_link = "College of Education";
  } else if (rfid_code == "04DC30A3") {
    name = "Kaeth Jelyn Anciado";
    identifier = "Visitor";
    id = "208223499";
    vehicle_type = "Four Wheels";
    vehicle_unit = "White Tesla Model Y";
    plate_number = "JJA 47032";
    owner_link = "https://raw.githubusercontent.com/Ze-r000/CvEX-U_Images/refs/heads/main/Visitor.png";
    department_link = "College of Nursing";
  } else {
    // No Records
    name = "No Records";
    identifier = "No Records";
    id = "No Records";
    vehicle_type = "No Records";
    vehicle_unit = "No Records";
    plate_number = "No Records";
    owner_link = "https://raw.githubusercontent.com/Ze-r000/CvEX-U_Images/refs/heads/main/P1.jpg";
    department_link = "No Records";
  }

  // Data to insert into Sheets
  sheet.insertRows(2);
  sheet.getRange("A2").setValue(date);
  sheet.getRange("B2").setValue(time);
  sheet.getRange("D2").setValue(name);
  sheet.getRange("E2").setValue(identifier);
  sheet.getRange("F2").setValue(id);
  sheet.getRange("G2").setValue(vehicle_type);
  sheet.getRange("H2").setValue(vehicle_unit);
  sheet.getRange("I2").setValue(plate_number);
  sheet.getRange("J2").setValue(rfid_code);
  sheet.getRange("K2").setValue(department_link);
  sheet.getRange("L2").setValue(owner_link);

  // Handle real-time data
  if (remarks == 0) {
    sheet.getRange("N3:Y3").clearContent();
    var remarkss = "Entry";
    sheet.getRange("C2").setValue(remarkss);
    sheet.getRange("N2").setValue(date);
    sheet.getRange("O2").setValue(time);
    sheet.getRange("P2").setValue(remarkss);
    sheet.getRange("Q2").setValue(name);
    sheet.getRange("R2").setValue(identifier);
    sheet.getRange("S2").setValue(id);
    sheet.getRange("T2").setValue(vehicle_type);
    sheet.getRange("U2").setValue(vehicle_unit);
    sheet.getRange("V2").setValue(plate_number);
    sheet.getRange("W2").setValue(rfid_code);
    sheet.getRange("X2").setValue(department_link);
    sheet.getRange("Y2").setValue(owner_link);
    sheet.getRange("AA3:AL3").copyTo(sheet.getRange("AA2:AL2"));
    sheet.getRange("AA:AL3").clearContent();
    count = 1;
    sheet.getRange("AM2").setValue(count);
  } else if (remarks == 1) {
    sheet.getRange("AA3:AL3").clearContent();
    var remarkss = "Exit";
    sheet.getRange("C2").setValue(remarkss);
    sheet.getRange("AA2").setValue(date);
    sheet.getRange("AB2").setValue(time);
    sheet.getRange("AC2").setValue(remarkss);
    sheet.getRange("AD2").setValue(name);
    sheet.getRange("AE2").setValue(identifier);
    sheet.getRange("AF2").setValue(id);
    sheet.getRange("AG2").setValue(vehicle_type);
    sheet.getRange("AH2").setValue(vehicle_unit);
    sheet.getRange("AI2").setValue(plate_number);
    sheet.getRange("AJ2").setValue(rfid_code);
    sheet.getRange("AK2").setValue(department_link);
    sheet.getRange("AL2").setValue(owner_link);
    sheet.getRange("N3:Y3").copyTo(sheet.getRange("N2:Y2"));
    sheet.getRange("N3:Y3").clearContent();
    count = -1;
    sheet.getRange("AM2").setValue(count);
  }
}

function autoRefresh() {
    SpreadsheetApp.flush(); // Forces changes to propagate
}
