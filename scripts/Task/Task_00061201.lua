--����Ľ�������
function Task_Accept_00061201()
	local player = GetPlayer();
	if player:GetLev() < 51 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61201) or task:HasCompletedTask(61201) or task:HasSubmitedTask(61201) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061201()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 51 then
		return false;
	end
	if task:HasAcceptedTask(61201) or task:HasCompletedTask(61201) or task:HasSubmitedTask(61201) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00061201()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61201) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061201(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61201) == npcId and Task_Accept_00061201 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61201
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "整治天池";
	elseif task:GetTaskSubmitNpc(61201) == npcId then
		if Task_Submit_00061201() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61201
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "整治天池";
		elseif task:HasAcceptedTask(61201) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61201
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "整治天池";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061201_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这点心是出自菜刀路之遥之手？这奇形怪状的点心看起来真不是好吃的样子，不过我尝了一小口，却发现其实是惊人的美味啊。";
	action.m_ActionMsg = "是啊，路之遥也住在这天池，他希望以后能和你和平相处。";
	return action;
end

function Task_00061201_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在天池能认识手艺这么好的人也不错，放心吧，我不会难为他的。但是让我恼火的是，这么漂亮的天池风景，竟然在废墟码头有一个渔人头目纠结了一大群奇怪的渔人在盘踞在那里，你去帮我把它们解决掉吧，我不会亏待你的。";
	action.m_ActionMsg = "……路之遥说的没错啊，那我去了。";
	return action;
end

function Task_00061201_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "没有那些讨厌的渔人，这片天池应该会更加风景宜人了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061201_step_table = {
		[1] = Task_00061201_step_01,
		[2] = Task_00061201_step_02,
		[10] = Task_00061201_step_10,
		};

function Task_00061201_step(step)
	if Task_00061201_step_table[step] ~= nil then
		return Task_00061201_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061201_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061201() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61201) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00061201_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(1302,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(61201) then
		return false;
	end

	if IsEquipTypeId(1302) then
		for k = 1, 1 do
			package:AddEquip(1302, 1);
		end
	else 
		package:AddItem(1302,1,1);
	end

	player:AddExp(60000);
	player:getCoin(47000);
	return true;
end

--��������
function Task_00061201_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61201);
end
