    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_800057A8
    cmpwi r3, 0x1
    beq @L_801342A4
    bge @L_80134294
    b @L_801342A4
    @L_80134294:
    cmpwi r3, 0x3
    bge @L_801342A4
    li r3, 0x115
    b @L_801342A8
    @L_801342A4:
    li r3, 0x2
    @L_801342A8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
