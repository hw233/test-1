--����Ľ�������
function Task_Accept_00031803()
	local player = GetPlayer();
	if player:GetLev() < 69 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31803) or task:HasCompletedTask(31803) or task:HasSubmitedTask(31803) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31802) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31802) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031803()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 69 then
		return false;
	end
	if task:HasAcceptedTask(31803) or task:HasCompletedTask(31803) or task:HasSubmitedTask(31803) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31802) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31802) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031803()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31803) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031803(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31803) == npcId and Task_Accept_00031803 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31803
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "上供额提";
	elseif task:GetTaskSubmitNpc(31803) == npcId then
		if Task_Submit_00031803() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31803
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "上供额提";
		elseif task:HasAcceptedTask(31803) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31803
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "上供额提";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031803_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "其实我并没有想要对严中的女儿怎样，只是有个人质在这里，可以让他不要那么的轻举妄动，不过我也猜的出来你来找我的用意。";
	action.m_ActionMsg = "是的，我希望你放过严中的女儿。";
	return action;
end

function Task_00031803_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "既然你真的希望这样，就去城郊的环魔金矿，杀掉那里的矿工，把他们的金矿抢过来给我，用严中的女儿换一些金子，我还是愿意的。";
	action.m_ActionMsg = "就是贪财啊。";
	return action;
end

function Task_00031803_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哦黄金矿黄金矿，金灿灿的宝啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031803_step_table = {
		[1] = Task_00031803_step_01,
		[2] = Task_00031803_step_02,
		[10] = Task_00031803_step_10,
		};

function Task_00031803_step(step)
	if Task_00031803_step_table[step] ~= nil then
		return Task_00031803_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031803_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031803() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31803) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15075,1);
	if itemNum ~= 0 then
		if itemNum > 12 then
			itemNum = 12;
			package:SetItem(15075, itemNum, 1);
		end
		task:AddTaskStep2(31803, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00031803_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15075,1) < 12 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(31803) then
		return false;
	end

	package:DelItemAll(15075,1);

	player:AddExp(220000);
	player:getCoin(204000);
	player:getTael(20);
	return true;
end

--��������
function Task_00031803_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15075,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31803);
end
