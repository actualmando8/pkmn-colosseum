    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x1
    stw r0, 0x14(r1)
    bl fn_801F453C
    rlwinm r0, r3, 0, 24, 31
    cmpwi r0, 0x1
    beq @L_8020FC44
    bge @L_8020FC38
    cmpwi r0, 0x0
    bge @L_8020FC4C
    b @L_8020FC5C
@L_8020FC38:
    cmpwi r0, 0x5
    bge @L_8020FC5C
    b @L_8020FC54
@L_8020FC44:
    li r3, 0x0
    b @L_8020FC60
@L_8020FC4C:
    li r3, 0x1
    b @L_8020FC60
@L_8020FC54:
    li r3, 0x2
    b @L_8020FC60
@L_8020FC5C:
    li r3, 0x0
@L_8020FC60:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
