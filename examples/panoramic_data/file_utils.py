def load_files():
    files = []
    attrs = {}
    attrs['Demographic'] = [{"Sex":"int"}, {"Age":"int"}, {"Race":"int"},
         {"Material Status":"int"}, {"Income":"int"}, {"Religion":"int"}]

    attrs['Physical'] = [{"Height":"double"}, {"Weight":"double"}, 
        {"BMI":"double"}, {"Coma (GSC)":"int"}, {"Skin":"int"}]

    attrs['Vitals'] = [{"Heart Rate":"int"}, {"Respiratory Rate":"double"}, 
        {"Blood Pressure":"int"}]

    attrs['Blood Tests'] = {}
    attrs['Blood Tests']['BMP'] = [{"NA+":"double"}, {"K+":"double"}, 
        {"Cl-":"double"}, {"Co2":"double"}, {"Ca":"double"}, 
        {"BUN":"double"}, {"Creatinine":"double"}, {"Glucose":"double"}]

    attrs['Blood Tests']['ABG'] = [{"pH":"double"}, {"H+":"double"}, 
        {"PaO2":"double"}, {"PaCO2":"double"}, {"HCO3-":"double"}, 
        {"SBLe":"double"}, {"BE":"double"}, {"Total CO2":"double"},
        {"Total O2":"double"}]

    attrs['Blood Tests']['CBC'] = [{"WBC":"double"}, {"RBC":"double"}, 
        {"Hgb":"double"}, {"Hct":"double"}, {"MCV":"double"}, 
        {"MCHC":"double"}, {"RDW":"double"}, {"Neuts":"double"},
        {"Lymphs":"double"}, {"Monos":"double"}, {"EOS":"double"},
        {"Basos":"double"}, {"Platlets":"double"}, 
        {"Prothrombin Time":"double"}]

    attrs['Blood Tests']['Cardiac'] = [{"CR-MB":"double"}, {"LDH":"double"}, 
        {"BNP":"double"}]

    attrs['Blood Tests']['Hepatic'] = [{"Triglycerides":"double"}, 
        {"Cholesterol":"double"}, {"LDL":"double"}, {"HDL":"double"}, 
        {"Bilirubin":"double"}]
    
    files.append({})
    files[0]['id'] = 0
    files[0]['name'] = "MIMIC"
    files[0]['attrs'] = attrs

    files.append({})
    files[1]['id'] = 1
    files[1]['name'] = "file1"
    files[1]['attrs'] = {"x":"float","y":"float"}

    files.append({})
    files[2]['id'] = 2
    files[2]['name'] = "file2"
    files[2]['attrs'] = {"group1":[{"a0":"float"},{"a1":"float"}], "group2":[{"a3":"float"},{"a4":"float"}]}
    files[2]['attrs']['group3'] = {}
    files[2]['attrs']['group3']['subgroup1'] = [{"a5":"float"},{"a6":"float"}] 
    files[2]['attrs']['group3']['subgroup2'] = [{"a7":"float"},{"a8":"float"}] 
    
    return files