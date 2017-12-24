
#Query 1:
SELECT count(PatientID)
FROM patient;

Select *
FROM employee
WHERE JobType = "Surgeon";

Select *
FROM employee;

Select *
FROM patient p, conditions c
WHERE p.Conditions = c.Conditions;

Select DISTINCT s.SurgeonID, s.TechID, s.NurseID
FROM employee e, schedules s
WHERE (s.SurgeonID = e.EmployeeID) AND (e.EmployeeID = 1268)
ORDER BY Date, Time;

Select Date, Time, Room
FROM schedules s
WHERE Room = 201
ORDER BY Date, Time;

Select SUM(c.Cost + r.Price + e.Cost - i.MaxAllowable)
FROM patient p, room r, employee e, schedules s, insurance i, conditions c
WHERE p.PatientID = 10000 AND s.PatientID = p.PatientID AND 
p.InsuranceID = i.InsuranceID AND s.SurgeonID = e.EmployeeID AND r.Room = s.Room
AND c.Conditions = p.Conditions AND s.TechID = e.EmployeeID;


Select p.PatientID, p.Name, e.Cost AS employeefee, c.Cost AS conditionfee, r.Price AS roomfee, i.MaxAllowable, ((e.Cost) + c.Cost + (r.Price)) AS Total
FROM patient p, employee e, schedules s, conditions c, room r, insurance i
WHERE p.PatientID = 10002 AND s.PatientID = p.PatientID AND (s.SurgeonID = e.EmployeeID OR s.TechID = e.EmployeeID OR e.EmployeeID = s.NurseID)
AND p.Conditions = c.Conditions AND r.Room = s.Room AND p.InsuranceID = i.InsuranceID;

Select p.PatientID, SUM(e.Cost) AS employeefee, c.Cost AS conditionfee, SUM(r.Price) AS roomfee, i.MaxAllowable, (SUM(e.Cost) + c.Cost + SUM(r.Price)) AS Total
FROM patient p, employee e, schedules s, conditions c, room r, insurance i
WHERE p.PatientID = 10002 AND s.PatientID = p.PatientID AND (s.SurgeonID = e.EmployeeID OR s.TechID = e.EmployeeID)
AND p.Conditions = c.Conditions AND r.Room = s.Room AND p.InsuranceID = i.InsuranceID;

Select p.PatientID, CAST(SUM(e.Cost)/2 AS SIGNED) AS employeefee, c.Cost AS conditionfee, CAST(SUM(r.Price)/2 AS SIGNED) AS roomfee, i.MaxAllowable, CAST((SUM(e.Cost)/2 + c.Cost - i.MaxAllowable + SUM(r.Price)/2) AS SIGNED) AS Total
FROM patient p, employee e, schedules s, conditions c, room r, insurance i
WHERE p.PatientID = 10002 AND s.PatientID = p.PatientID AND (s.SurgeonID = e.EmployeeID OR s.TechID = e.EmployeeID)
AND p.Conditions = c.Conditions AND r.Room = s.Room AND p.InsuranceID = i.InsuranceID;

Select *
FROM schedules s
Where PatientID = 10002;

Select *
FROM employee;

Select *
FROM patient;

Select Date, Time, e.Name, e.JobType, e.EmployeeID, Room
FROM employee e, schedules s
WHERE (s.SurgeonID = e.EmployeeID) AND (e.EmployeeID = 1268)
ORDER BY Date, Time;