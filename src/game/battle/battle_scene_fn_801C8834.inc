    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    rlwinm r30, r29, 0, 16, 31
    li r31, 0x0
    subi r0, r30, 0x200
    cmplwi r0, 0x11
    bgt @L_801C88C0
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -8748
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r31, 0x99
    b @L_801C88C0
    li r31, 0x9c
    b @L_801C88C0
    li r31, 0x9f
    b @L_801C88C0
    li r31, 0xb9
    b @L_801C88C0
    li r31, 0xc8
    b @L_801C88C0
    li r31, 0x11f
    b @L_801C88C0
    li r31, 0xf3
    b @L_801C88C0
    li r31, 0xf4
    b @L_801C88C0
    li r31, 0xf5
@L_801C88C0:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_801C88D4
    li r3, 0x0
    b @L_801C89DC
@L_801C88D4:
    mr r3, r31
    bl fn_800756C8
    subi r0, r30, 0x200
    cmplwi r0, 0x10
    bgt @L_801C8948
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -8816
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r30, 0x201
    b @L_801C894C
    li r30, 0x203
    b @L_801C894C
    li r30, 0x205
    b @L_801C894C
    li r30, 0x207
    b @L_801C894C
    li r30, 0x209
    b @L_801C894C
    li r30, 0x20b
    b @L_801C894C
    li r30, 0x20d
    b @L_801C894C
    li r30, 0x20f
    b @L_801C894C
    li r30, 0x211
    b @L_801C894C
@L_801C8948:
    li r30, 0x0
@L_801C894C:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x0
    beq @L_801C89D8
    li r0, -0x1
    cmpwi r0, 0x0
    ble @L_801C8980
    lis r5, 0x1
    rlwinm r4, r29, 0, 16, 31
    li r3, 0x0
    li r6, -0x1
    subi r5, r5, 0x1
    bl fn_80129A78
    b @L_801C8998
@L_801C8980:
    bge @L_801C8998
    rlwinm r4, r29, 0, 16, 31
    li r3, 0x0
    li r5, 0x1
    li r6, -0x1
    bl fn_801299C8
@L_801C8998:
    li r0, 0x1
    cmpwi r0, 0x0
    ble @L_801C89BC
    rlwinm r4, r30, 0, 16, 31
    li r3, 0x0
    li r5, 0x1
    li r6, -0x1
    bl fn_80129A78
    b @L_801C89D8
@L_801C89BC:
    bge @L_801C89D8
    lis r5, 0x1
    rlwinm r4, r30, 0, 16, 31
    li r3, 0x0
    li r6, -0x1
    subi r5, r5, 0x1
    bl fn_801299C8
@L_801C89D8:
    mr r3, r31
@L_801C89DC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
