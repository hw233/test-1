--����Ľ�������
function Task_Accept_00030503()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30503) or task:HasCompletedTask(30503) or task:HasSubmitedTask(30503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30502) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030503()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(30503) or task:HasCompletedTask(30503) or task:HasSubmitedTask(30503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30502) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030503()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30503) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030503(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30503) == npcId and Task_Accept_00030503 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30503
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "阻止刺客";
	elseif task:GetTaskSubmitNpc(30503) == npcId then
		if Task_Submit_00030503() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30503
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "阻止刺客";
		elseif task:HasAcceptedTask(30503) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30503
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "阻止刺客";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030503_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士我们又见面了，感谢你带来了随形的资助，这些钱对于我们的确很重要，不过可苦了随形，一个人在那里默默的开店支持我，我也是爱着她的，可是碍于师门，况且我们东山派的重任一日没有完成，我就一日不能儿女情长。";
	action.m_ActionMsg = "原来你们是同一师门的？";
	return action;
end

function Task_00030503_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是的，随形和我的师傅，那个隐居的守墓人，当年收留我们，并为我们起这样的名字，是希望我们互相帮助，如今世道混乱，我也只能以大局为重了。现在夜摩盟的邪恶势力当道，随形现在也处于危险之中，黑森林里的虚无林里埋伏了很多僵尸刺客，我刚得到消息，他们企图偷袭山河郡，怕是随形要遇到危险，你去帮我打败他们保护随形的安全吧。";
	action.m_ActionMsg = "您的确是很英明的帮主。";
	return action;
end

function Task_00030503_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么快就解决掉了？希望不是太晚，随形还没有遇到危险。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030503_step_table = {
		[1] = Task_00030503_step_01,
		[2] = Task_00030503_step_02,
		[10] = Task_00030503_step_10,
		};

function Task_00030503_step(step)
	if Task_00030503_step_table[step] ~= nil then
		return Task_00030503_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030503_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030503() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30503) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030503_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30503) then
		return false;
	end


	player:AddExp(10000);
	player:getCoin(13500);
	player:getTael(20);
	return true;
end

--��������
function Task_00030503_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30503);
end
