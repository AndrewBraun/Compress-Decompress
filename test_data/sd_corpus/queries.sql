--find the names of all volunteers at event 1
SELECT Name from Member
	WHERE ID in (SELECT Volunteer_ID from volunteers_at
							WHERE Event_ID = 1);

--find the emails of all employees on campaign 1
SELECT Name from Member
	WHERE ID in (SELECT Employee_ID from works_at
							WHERE Campaign_ID = 1);

--find the average donation amount
SELECT Avg(Amount) from Cash_flow
	WHERE Description = 'Donation';

--find names of all donors who have donated multiple times
SELECT Name from Member
	WHERE ID in (SELECT d1.Donor_ID from donates d1, donates d2
							WHERE d1.Donor_ID = d2.Donor_ID 
													and d1.Trans_ID <> d2.Trans_ID );

--find all names of volunteers and employees working on campaign 2 or events for campaign 2
(SELECT Name from Member
		WHERE ID in (SELECT Employee_ID FROM works_at WHERE Campaign_id = 2))
	union
	(SELECT Name from Member
		    WHERE ID in (SELECT Volunteer_ID FROM volunteers_at where Event_ID in 
			                 (SELECT Event_ID from Event where Campaign_id = 2)));

--find the emails of all volunteers of tier 2
SELECT Email from Member
	WHERE Member.Tier = 2;

--find the emails and names of all employees and volunteers who have not donated
SELECT Email, Name from Member
	WHERE ID in ((SELECT ID from Member) except all (SELECT Donor_ID from donates));
	
--list all campagins which are underway and the corresponding locations of events which have or are going to take place
SELECT Location, Campaign_id from Event
WHERE Campaign_id in (SELECT Campaign_ID from Campaign
					WHERE Completed = FALSE);

--largest cost associated with campaign 1
SELECT min(Amount) from Cash_flow 
        WHERE Transaction_id in (SELECT Trans_ID FROM Transact
		                                 WHERE Campaign_ID = 1);   

--list the start time of campagins which host events in 'Tillcum'
SELECT Start_Date, ID from Campaign where ID in (SELECT Campaign_id FROM Event
	  WHERE Location = 'Tillicum'); 

--find the start date and location of events in campaign 2
SELECT Start_Date, Location FROM Event
  WHERE Campaign_Id = 2;

--find all positive cash flows from campaign 1
(SELECT Amount from Cash_flow
	  where Amount > 0)
  intersect 
  (SELECT Amount from Cash_flow 
	    where Transaction_id in (SELECT Trans_ID from Transact 
		                              WHERE Campaign_ID = 1));



