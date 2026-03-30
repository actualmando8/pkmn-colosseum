    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x2
    bl fn_801C6908
    cmpwi r3, 0x1
    beq @L_801C49D8
    bge @L_801C49D8
    cmpwi r3, 0x0
    bge @L_801C49C8
    b @L_801C49D8
    @L_801C49C8:
    lis r3, fn_801C5F6C@ha
    addi r3, r3, fn_801C5F6C@l
    bl fn_801C431C
    b @L_801C49E4
    @L_801C49D8:
    lis r3, fn_801C5ED0@ha
    addi r3, r3, fn_801C5ED0@l
    bl fn_801C431C
    @L_801C49E4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
