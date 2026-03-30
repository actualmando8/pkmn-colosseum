    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_80478940(r13)
    cmpwi r0, 0x1
    beq @L_8007C2A0
    bge @L_8007C2A8
    cmpwi r0, 0x0
    bge @L_8007C294
    b @L_8007C2A8
@L_8007C294:
    li r3, 0xb
    bl fn_8002DC6C
    b @L_8007C2A8
@L_8007C2A0:
    li r3, 0xc
    bl fn_8002DC6C
@L_8007C2A8:
    li r3, 0x395
    bl fn_800FF58C
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
