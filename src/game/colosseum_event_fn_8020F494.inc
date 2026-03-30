    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x1
    stw r0, 0x14(r1)
    bl fn_801F453C
    rlwinm r0, r3, 0, 24, 31
    cmpwi r0, 0x2
    beq @L_8020F4E4
    bge @L_8020F4CC
    cmpwi r0, 0x0
    beq @L_8020F4DC
    bge @L_8020F4F4
    b @L_8020F504
@L_8020F4CC:
    cmpwi r0, 0x4
    beq @L_8020F4EC
    bge @L_8020F504
    b @L_8020F4FC
@L_8020F4DC:
    li r3, 0x0
    b @L_8020F508
@L_8020F4E4:
    li r3, 0x1
    b @L_8020F508
@L_8020F4EC:
    li r3, 0x2
    b @L_8020F508
@L_8020F4F4:
    li r3, 0x3
    b @L_8020F508
@L_8020F4FC:
    li r3, 0x4
    b @L_8020F508
@L_8020F504:
    li r3, 0x0
@L_8020F508:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
