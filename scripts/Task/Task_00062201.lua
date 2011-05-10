--����Ľ�������
function Task_Accept_00062201()
	local player = GetPlayer();
	if player:GetLev() < 73 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62201) or task:HasCompletedTask(62201) or task:HasSubmitedTask(62201) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062201()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 73 then
		return false;
	end
	if task:HasAcceptedTask(62201) or task:HasCompletedTask(62201) or task:HasSubmitedTask(62201) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00062201()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62201) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062201(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62201) == npcId and Task_Accept_00062201 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62201
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "复仇人生";
	elseif task:GetTaskSubmitNpc(62201) == npcId then
		if Task_Submit_00062201() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62201
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "复仇人生";
		elseif task:HasAcceptedTask(62201) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62201
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "复仇人生";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062201_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "路漫漫，人惶惶，复仇之路何其长。勇士你可知道家破人亡的滋味？我年轻的时候因为遭人暗算，失去了年轻的妻子和年幼的女儿，气愤的我从此踏上了复仇之路，我一路苦练武功并追杀当初利用我的夜叉王至此地，可是却一直无果。\n";
	action.m_ActionMsg = "他已经是夜摩盟的首领了，的确不好对付。";
	return action;
end

function Task_00062201_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "但是又一次我偶然在海市桥的绿波宫见到一位年轻的女子，神情与我去世的妻子十分相像，一切恍如隔世，我突然觉得我的女儿可能还活着。可是我又不好意思直接去问，能不能麻烦勇士你去帮我打听一下呢？\n";
	action.m_ActionMsg = "原来你也是个杯具人物啊，那我帮你去问问吧。";
	return action;
end

function Task_00062201_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "前世无知，后世可修，勇士来此有何贵干？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062201_step_table = {
		[1] = Task_00062201_step_01,
		[2] = Task_00062201_step_02,
		[10] = Task_00062201_step_10,
		};

function Task_00062201_step(step)
	if Task_00062201_step_table[step] ~= nil then
		return Task_00062201_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062201_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062201() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62201) then
		return false;
	end
	task:AddTaskStep(62201);
	return true;
end



--�ύ����
function Task_00062201_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62201) then
		return false;
	end


	player:AddExp(280000);
	player:getCoin(259000);
	return true;
end

--��������
function Task_00062201_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62201);
end
