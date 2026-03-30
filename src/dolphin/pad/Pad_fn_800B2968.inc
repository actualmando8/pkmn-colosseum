    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    mr r30, r3
    bl fn_800B2220
    cmpwi r3, 0x0
    bge @L_800B2990
    b @L_800B29DC
    @L_800B2990:
    addi r3, r30, 0x0
    li r4, 0x0
    bl fn_800B24A4
    addi r31, r3, 0x0
    addi r3, r30, 0x0
    li r4, 0x0
    bl fn_800B26E4
    add r0, r31, r3
    cmpwi r0, 0x1
    beq @L_800B29D0
    bge @L_800B29D8
    cmpwi r0, 0x0
    bge @L_800B29C8
    b @L_800B29D8
    @L_800B29C8:
    li r3, 0x0
    b @L_800B29DC
    @L_800B29D0:
    li r3, -0x6
    b @L_800B29DC
    @L_800B29D8:
    li r3, -0x6
    @L_800B29DC:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
