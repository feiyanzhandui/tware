select icd9.subject_id as id,
       code as icd9,
       IFNULL(sex,0) as sex,
       IFNULL(age,65) as age,--
       IFNULL(race,-1) as race,
       IFNULL(marital,-1) as marital,
       IFNULL(religion,-1) as religion,
       IFNULL(weight,60) as weight,
       IFNULL(temperature,98.6) as temperature,
       IFNULL(heart_rate,100) as heart_rate,
       IFNULL(resp_rate,20) as resp_rate,
       IFNULL(systolic_bp,120) as systolic_bp,
       IFNULL(diastolic_bp,80) as diastolic_bp,
       IFNULL(spo2,100) as spo2,
       IFNULL(gsc,15) as gsc,
       IFNULL(braden,23) as braden,
       IFNULL(sofa,0) as sofa,--
       IFNULL(sodium,141) as sodium,
       IFNULL(potassium,4.3) as potassium,
       IFNULL(chloride,102.5) as chloride,
       IFNULL(magnesium,1.9) as magnesium,
       IFNULL(calcium,9.45) as calcium,
       IFNULL(anion_gap,6) as anion_gap,
       IFNULL(bun,14) as bun,
       IFNULL(creatinine,case sex
                             when 0 then 1
                             when 1 then 0.85
                         end) as creatinine,
       IFNULL(glucose,80) as glucose,
       IFNULL(ph,7.4) as ph,
       IFNULL(be,0) as be,
       IFNULL(total_co2,26.5) as total_co2,
       IFNULL(total_o2,90) as total_o2,--
       IFNULL(pco2,40) as pco2,
       IFNULL(po2,87.5) as po2,
       IFNULL(wbc,7.25) as wbc,--
       IFNULL(rbc,4.8) as rbc,--
       IFNULL(hgb,14) as hgb,--
       IFNULL(hct,42) as hct,--
       IFNULL(mcv,90) as mcv,--
       IFNULL(mch,30) as mch,--
       IFNULL(mchc,33) as mchc,--
       IFNULL(rdw,13) as rdw,
       IFNULL(plates,250) as plates,--
       IFNULL(neuts,60) as neuts,--
       IFNULL(lymphs,30.5) as lymphs,
       IFNULL(monos,6.5) as monos,
       IFNULL(basos,0.375) as basos,--
       IFNULL(eos,2) as eos,
       IFNULL(pt,13) as pt,
       IFNULL(inr_pt,1) as inr_pt,
       IFNULL(ptt,32) as ptt,
       IFNULL(ckmb,5) as ckmb,--
       IFNULL(cpkmb,80) as cpkmb,--
       IFNULL(ldh,0) as ldh,--
       IFNULL(bnp,400) as bnp,
       IFNULL(tropi,0) as tropi,
       IFNULL(tropt,0) as tropt,
       IFNULL(total_bili,0.75) as total_bili,
       IFNULL(direct_bili,0.2) as direct_bili,
       IFNULL(indirect_bili,0.55) as indirect_bili,
       IFNULL(albumin,4.5) as albumin,
       IFNULL(tg,102) as tg--
from (
    select 123456 as subject_id,
           null as code   
) as icd9
-- patient
left outer join (
    select subject_id,
           case sex
               when 'M' then 0
               when 'F' then 1
               else null
           end as sex,
           FLOOR(DATEDIFF(dod,dob)/365) as age
    from d_patients
) as patient on patient.subject_id = icd9.subject_id
-- demographic
left outer join (
    select subject_id,
           case ethnicity_descr
               when 'AMERICAN INDIAN/ALASKA NATIVE' then 0
               when 'ASIAN' then 1
               when 'ASIAN - CHINESE' then 2
               when 'ASIAN - VIETNAMESE' then 3
               when 'BLACK/AFRICAN AMERICAN' then 4
               when 'BLACK/CAPE VERDEAN' then 5
               when 'BLACK/HAITIAN' then 6
               when 'HISPANIC OR LATINO' then 7
               when 'HISPANIC/LATINO - PUERTO RICAN' then 8
               when 'MULTI RACE ETHNICITY' then 9
               when 'NATIVE HAWAIIAN OR OTHER PACIFIC ISLAND' then 10
               when 'OTHER' then 11
               when 'PATIENT DECLINED TO ANSWER' then 12
               when 'UNABLE TO OBTAIN' then 13
               when 'UNKNOWN/NOT SPECIFIED' then 14
               when 'WHITE' then 15
               when 'WHITE - RUSSIAN' then 16
               else null
           end as race,
           case marital_status_descr
               when 'DIVORCED' then 0
               when 'MARRIED' then 1
               when 'SEPARATED' then 2
               when 'SINGLE' then 3
               when 'UNKNOWN (DEFAULT)' then 4
               when 'WIDOWED' then 5
               else null
           end as marital,
           case religion_descr
               when '7TH DAY ADVENTIST' then 0
               when 'BAPTIST' then 1
               when 'BUDDHIST' then 2
               when 'CATHOLIC' then 3
               when 'CHRISTIAN SCIENTIST' then 4
               when 'EPISCOPALIAN' then 5
               when 'GREEK ORTHODOX' then 6
               when 'HEBREW' then 7
               when 'HINDU' then 8
               when 'JEHOVAH\'S WITNESS' then 9
               when 'JEWISH' then 10
               when 'LUTHERAN' then 11
               when 'METHODIST' then 12
               when 'MUSLIM' then 13
               when 'NOT SPECIFIED' then 14
               when 'OTHER' then 15
               when 'PROTESTANT QUAKER' then 16
               when 'ROMANIAN EAST. ORTH' then 17
               when 'UNITARIAN-UNIVERSALIST' then 18
               when 'UNOBTAINABLE' then 19
               else null
            end as religion
    from demographic_detail
) as demographic on demographic.subject_id = icd9.subject_id
-- weight
left outer join (
    select subject_id,
           ROUND(value1num) as weight
    from chartevents
    where itemid in (580,581,763)
    group by subject_id
    having MIN(realtime)
) as weight on weight.subject_id = icd9.subject_id
-- temperature
left outer join (
    select subject_id,
           ROUND(value1num, 1) as temperature
    from chartevents
    where itemid = 678
    group by subject_id
    having MIN(realtime)
) as temperature on temperature.subject_id = icd9.subject_id
-- heart_rate
left outer join (
    select subject_id,
           value1num as heart_rate
    from chartevents
    where itemid = 211
    group by subject_id
    having MIN(realtime)
) as heart_rate on heart_rate.subject_id = icd9.subject_id
-- resp_rate
left outer join (
    select subject_id,
           value1num as resp_rate
    from chartevents
    where itemid = 618
    group by subject_id
    having MIN(realtime)
) as resp_rate on resp_rate.subject_id = icd9.subject_id
-- blood pressure
left outer join (
    select subject_id,
           value1num as systolic_bp,
           value2num as diastolic_bp
    from chartevents
    where itemid = 51
    group by subject_id
    having MIN(realtime)
) as bp on bp.subject_id = icd9.subject_id
-- spo2
left outer join (
    select subject_id,
           ROUND(value1num) as spo2
    from chartevents
    where itemid = 646
    group by subject_id
    having MIN(realtime)
) as spo2 on spo2.subject_id = icd9.subject_id
-- gsc
left outer join (
    select subject_id,
           value1num as gsc
    from chartevents
    where itemid = 198
    group by subject_id
    having MIN(realtime)
) as gsc on gsc.subject_id = icd9.subject_id
-- braden
left outer join (
    select subject_id,
           value1num as braden
    from chartevents
    where itemid = 87
    group by subject_id
    having MIN(realtime)
) as braden on braden.subject_id = icd9.subject_id
-- sofa
left outer join (
    select subject_id,
           value1num as sofa
    from chartevents
    where itemid = 20009
    group by subject_id
    having MIN(realtime)
) as sofa on sofa.subject_id = icd9.subject_id
-- sodium
left outer join (
    select subject_id,
           IFNULL(valuenum,case
                               when value like '%>%' then 180
                               else null
                           end) as sodium
    from labevents
    where itemid = 50159
    group by subject_id
    having MIN(charttime)
) as sodium on sodium.subject_id = icd9.subject_id
-- potassium
left outer join (
    select subject_id,
           IFNULL(valuenum,case
                               when value like '%GREATER%' then 10
                               when value like '%>%' then 10
                               when value like '%LESS%' then 1.5
                               else null
                           end) as potassium
    from labevents
    where itemid = 50149
    group by subject_id
    having MIN(charttime)
) as potassium on potassium.subject_id = icd9.subject_id
-- chloride
left outer join (
    select subject_id,
           IFNULL(valuenum,case
                               when value like '%GREATER%' then 140
                               when value like '%>%' then 140
                               else null
                           end) as chloride
    from labevents
    where itemid = 50083
    group by subject_id
    having MIN(charttime)
) as chloride on chloride.subject_id = icd9.subject_id
-- magnesium
left outer join (
    select subject_id,
           IFNULL(valuenum,case
                               when value like '%LESS%' then 1
                               else null
                           end) as magnesium
    from labevents
    where itemid = 50140
    group by subject_id
    having MIN(charttime)
) as magnesium on magnesium.subject_id = icd9.subject_id
-- calcium
left outer join (
    select subject_id,
           IFNULL(valuenum,case
                               when value like '%LESS%' then 0
                               else null
                           end) as calcium
    from labevents
    where itemid = 50079
    group by subject_id
    having MIN(charttime)
) as calcium on calcium.subject_id = icd9.subject_id
-- anion_gap
left outer join (
    select subject_id,
           valuenum as anion_gap
    from labevents
    where itemid = 50068
    group by subject_id
    having MIN(charttime)
) as anion_gap on anion_gap.subject_id = icd9.subject_id
-- bun
left outer join (
    select subject_id,
           valuenum as bun
    from labevents
    where itemid = 50177
    group by subject_id
    having MIN(charttime)
) as bun on bun.subject_id = icd9.subject_id
-- creatinine
left outer join (
    select subject_id,
           IFNULL(valuenum,case
                               when value like '%LESS%' then 0
                               when value like '%<%' then 0
                               else null
                           end) as creatinine
    from labevents
    where itemid = 50090
    group by subject_id
    having MIN(charttime)
) as creatinine on creatinine.subject_id = icd9.subject_id
-- glucose
left outer join (
    select subject_id,
           IFNULL(valuenum,case
                               when value like '%GREATER%' then 1300
                               else null
                           end) as glucose
    from labevents
    where itemid = 50112
    group by subject_id
    having MIN(charttime)
) as glucose on glucose.subject_id = icd9.subject_id
-- ph
left outer join (
    select subject_id,
           valuenum as ph
    from labevents
    where itemid = 50018
    group by subject_id
    having MIN(charttime)
) as ph on ph.subject_id = icd9.subject_id
-- be
left outer join (
    select subject_id,
           valuenum as be
    from labevents
    where itemid = 50002
    group by subject_id
    having MIN(charttime)
) as be on be.subject_id = icd9.subject_id
-- total_co2
left outer join (
    select subject_id,
           IFNULL(valuenum,case
                               when value like '%GREATER%' then 40
                               when value like '%>%' then 40
                               when value like '%LESS%' then 5
                               when value like '%<%' then 5
                               else null
                           end) as total_co2
    from labevents
    where itemid = 50172
    group by subject_id
    having MIN(charttime)
) as total_co2 on total_co2.subject_id = icd9.subject_id
-- total_o2
left outer join (
    select subject_id,
           valuenum as total_o2
    from labevents
    where itemid = 50015
    group by subject_id
    having MIN(charttime)
) as total_o2 on total_o2.subject_id = icd9.subject_id
-- pco2
left outer join (
    select subject_id,
           valuenum as pco2
    from labevents
    where itemid = 50016
    group by subject_id
    having MIN(charttime)
) as pco2 on pco2.subject_id = icd9.subject_id
-- po2
left outer join (
    select subject_id,
           valuenum as po2
    from labevents
    where itemid = 50019
    group by subject_id
    having MIN(charttime)
) as po2 on po2.subject_id = icd9.subject_id
-- wbc
left outer join (
    select subject_id,
           IFNULL(ROUND(valuenum,1),case
                                        when value like '%<%' then 0
                                        else null
                                    end) as wbc
    from labevents
    where itemid = 50468
    group by subject_id
    having MIN(charttime)
) as wbc on wbc.subject_id = icd9.subject_id
-- rbc
left outer join (
    select subject_id,
           ROUND(valuenum,2) as rbc
    from labevents
    where itemid = 50442
    group by subject_id
    having MIN(charttime)
) as rbc on rbc.subject_id = icd9.subject_id
-- hgb
left outer join (
    select subject_id,
           ROUND(valuenum,1) as hgb
    from labevents
    where itemid = 50386
    group by subject_id
    having MIN(charttime)
) as hgb on hgb.subject_id = icd9.subject_id
-- hct
left outer join (
    select subject_id,
           ROUND(valuenum,1) as hct
    from labevents
    where itemid = 50383
    group by subject_id
    having MIN(charttime)
) as hct on hct.subject_id = icd9.subject_id
-- mcv
left outer join (
    select subject_id,
           ROUND(valuenum,1) as mcv
    from labevents
    where itemid = 50413
    group by subject_id
    having MIN(charttime)
) as mcv on mcv.subject_id = icd9.subject_id
-- mch
left outer join (
    select subject_id,
           ROUND(valuenum,1) as mch
    from labevents
    where itemid = 50411
    group by subject_id
    having MIN(charttime)
) as mch on mch.subject_id = icd9.subject_id
-- mchc
left outer join (
    select subject_id,
           ROUND(valuenum,1) as mchc
    from labevents
    where itemid = 50412
    group by subject_id
    having MIN(charttime)
) as mchc on mchc.subject_id = icd9.subject_id
-- rdw
left outer join (
    select subject_id,
           ROUND(valuenum,1) as rdw
    from labevents
    where itemid = 50444
    group by subject_id
    having MIN(charttime)
) as rdw on rdw.subject_id = icd9.subject_id
-- plates
left outer join (
    select subject_id,
           IFNULL(ROUND(valuenum,1),case
                                        when value like '%<%' then 5
                                        else null
                                    end) as plates
    from labevents
    where itemid = 50428
    group by subject_id
    having MIN(charttime)
) as plates on plates.subject_id = icd9.subject_id
-- neuts
left outer join (
    select subject_id,
           ROUND(valuenum,1) as neuts
    from labevents
    where itemid = 50419
    group by subject_id
    having MIN(charttime)
) as neuts on neuts.subject_id = icd9.subject_id
-- lymphs
left outer join (
    select subject_id,
           ROUND(valuenum,1) as lymphs
    from labevents
    where itemid = 50408
    group by subject_id
    having MIN(charttime)
) as lymphs on lymphs.subject_id = icd9.subject_id
-- monos
left outer join (
    select subject_id,
           ROUND(valuenum,1) as monos
    from labevents
    where itemid = 50417
    group by subject_id
    having MIN(charttime)
) as monos on monos.subject_id = icd9.subject_id
-- basos
left outer join (
    select subject_id,
           ROUND(valuenum,1) as basos
    from labevents
    where itemid = 50333
    group by subject_id
    having MIN(charttime)
) as basos on basos.subject_id = icd9.subject_id
-- eos
left outer join (
    select subject_id,
           ROUND(valuenum,1) as eos
    from labevents
    where itemid = 50373
    group by subject_id
    having MIN(charttime)
) as eos on eos.subject_id = icd9.subject_id
-- pt
left outer join (
    select subject_id,
           IFNULL(ROUND(valuenum,1),case
                                        when value like '%GREATER%' then 125
                                        when value like '%HIGHEST%' then 125
                                        when value like '%>%' then 125
                                        else null
                                    end) as pt
    from labevents
    where itemid = 50439
    group by subject_id
    having MIN(charttime)
) as pt on pt.subject_id = icd9.subject_id
-- inr_pt
left outer join (
    select subject_id,
           IFNULL(ROUND(valuenum,1),case
                                        when value like '%GREATER%' then 44
                                        when value like '%>%' then 44
                                        else null
                                    end) as inr_pt
    from labevents
    where itemid = 50399
    group by subject_id
    having MIN(charttime)
) as inr_pt on inr_pt.subject_id = icd9.subject_id
-- ptt
left outer join (
    select subject_id,
           IFNULL(ROUND(valuenum,1),case
                                        when value like '%HIGHEST%' then 150
                                        when value like '%>%' then 150
                                        else null
                                    end) as ptt
    from labevents
    where itemid = 50440
    group by subject_id
    having MIN(charttime)
) as ptt on ptt.subject_id = icd9.subject_id
-- ckmb
left outer join (
    select subject_id,
           IFNULL(valuenum,case
                               when value like '%GREATER%' then 600
                               else null
                           end) as ckmb
    from labevents
    where itemid = 50087
    group by subject_id
    having MIN(charttime)
) as ckmb on ckmb.subject_id = icd9.subject_id
-- cpkmb
left outer join (
    select subject_id,
           valuenum as cpkmb
    from labevents
    where itemid = 50086
    group by subject_id
    having MIN(charttime)
) as cpkmb on cpkmb.subject_id = icd9.subject_id
-- ldh
left outer join (
    select subject_id,
           valuenum as ldh
    from labevents
    where itemid = 50134
    group by subject_id
    having MIN(charttime)
) as ldh on ldh.subject_id = icd9.subject_id
-- bnp
left outer join (
    select subject_id,
           IFNULL(valuenum,case
                               when value like '%GREATER%' then 35000
                               when value like '%>%' then 35000
                               else null
                           end) as bnp
    from labevents
    where itemid = 50195
    group by subject_id
    having MIN(charttime)
) as bnp on bnp.subject_id = icd9.subject_id
-- tropi
left outer join (
    select subject_id,
           IFNULL(ROUND(valuenum,1),case
                                        when value like '%GREATER%' then 50
                                        when value like '%>%' then 50
                                        else null
                                    end) as tropi
    from labevents
    where itemid = 50188
    group by subject_id
    having MIN(charttime)
) as tropi on tropi.subject_id = icd9.subject_id
-- tropt
left outer join (
    select subject_id,
           IFNULL(ROUND(valuenum,2),case
                                        when value like '%GREATER%' then 25
                                        when value like '%>%' then 25
                                        else null
                                    end) as tropt
    from labevents
    where itemid = 50189
    group by subject_id
    having MIN(charttime)
) as tropt on tropt.subject_id = icd9.subject_id
-- total_bili
left outer join (
    select subject_id,
           ROUND(valuenum,1) as total_bili
    from labevents
    where itemid = 50170
    group by subject_id
    having MIN(charttime)
) as total_bili on total_bili.subject_id = icd9.subject_id
-- direct_bili
left outer join (
    select subject_id,
           IFNULL(ROUND(valuenum,1),case
                                        when value like '%LESS%' then 0
                                        when value like '%<%' then 0
                                        else null
                                    end) as direct_bili
    from labevents
    where itemid = 50096
    group by subject_id
    having MIN(charttime)
) as direct_bili on direct_bili.subject_id = icd9.subject_id
-- indirect_bili
left outer join (
    select subject_id,
           IFNULL(ROUND(valuenum,1),case
                                        when value like '%LESS%' then 0
                                        when value like '%<%' then 0
                                        else null
                                    end) as indirect_bili
    from labevents
    where itemid = 50127
    group by subject_id
    having MIN(charttime)
) as indirect_bili on indirect_bili.subject_id = icd9.subject_id
-- albumin
left outer join (
    select subject_id,
           IFNULL(valuenum,case
                               when value like '%LESS%' then 0
                               when value like '%<%' then 0
                               else null
                           end) as albumin
    from labevents
    where itemid = 50060
    group by subject_id
    having MIN(charttime)
) as albumin on albumin.subject_id = icd9.subject_id
-- tg
left outer join (
    select subject_id,
           valuenum as tg
    from labevents
    where itemid = 50174
    group by subject_id
    having MIN(charttime)
) as tg on tg.subject_id = icd9.subject_id
-- filter rules
into outfile '/tmp/mimic2_healthy.csv'
fields terminated by ','
lines terminated by '\n';